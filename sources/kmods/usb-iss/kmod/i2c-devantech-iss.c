/*
 * Driver for the Devantech USB-ISS Multifunction Communications Module
 *
 * Copyright (c) 2011 Ericsson AB
 *
 * Derived from:
 *  i2c-diolan-u2c.c
 *  Copyright (c) 2010-2011 Ericsson AB
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <linux/i2c.h>

#define DRIVER_NAME		"i2c-devantech-iss"

#define USB_VENDOR_ID_DEVANTECH	0x04d8
#define USB_DEVICE_ID_ISS	0xffee

/* commands */
#define ISS_CMD_I2C_DIRECT	0x57	/* Custom I2C sequence */
#define ISS_CMD_I2C_INTERNAL	0x5a	/* internal commands */

/* direct commands */
#define ISS_I2C_START		0x01
#define ISS_I2C_RESTART		0x02
#define ISS_I2C_STOP		0x03
#define ISS_I2C_NACK		0x04
#define ISS_I2C_READ(n)		(0x20 + (n) - 1)
#define ISS_I2C_WRITE(n)	(0x30 + (n) - 1)

/* internal commands */
#define ISS_GET_VERSION		0x01
#define ISS_MODE		0x02
#define ISS_GET_SERIAL		0x03

/* modes */
#define ISS_MODE_I2C_S_20KHZ	0x20	/* I2C, SW (bitbang), 20 kHz clock */
#define ISS_MODE_I2C_S_50KHZ	0x30	/* I2C, SW (bitbang), 50 kHz clock */
#define ISS_MODE_I2C_S_100KHZ	0x40	/* I2C, SW (bitbang), 100 kHz clock */
#define ISS_MODE_I2C_S_400KHZ	0x50	/* I2C, SW (bitbang), 400 kHz clock */
#define ISS_MODE_I2C_H_100KHZ	0x60	/* I2C, HW (PIC), 100 kHz clock */
#define ISS_MODE_I2C_H_400KHZ	0x70	/* I2C, HW (PIC), 400 kHz clock */
#define ISS_MODE_I2C_H_1000KHZ	0x80	/* I2C, HW (PIC), 1000 kHz clock */

/* response codes */

#define ISS_RESP_ACK		0xff
#define ISS_RESP_NACK		0x00

#define ISS_RESP_OK		0x00
#define ISS_RESP_DEV_ERROR	0x01
#define ISS_RESP_OVERFLOW	0x02
#define ISS_RESP_UNDERFLOW	0x03
#define ISS_RESP_UNKNOWN_CMD	0x04

#define ISS_USB_TIMEOUT		100	/* in ms */

#define ISS_MAX_TRANSFER_LEN	60

/* Structure to hold all of our device specific stuff */
struct i2c_devantech_iss {
	u8 buffer[ISS_MAX_TRANSFER_LEN];/* rx and tx buffer */
	struct usb_interface *usb_if;	/* device data interface */
	struct usb_interface *interface;/* device control interface */
	struct usb_device *usb_dev;	/* the usb device for this device */
	struct i2c_adapter adapter;	/* i2c related things */
	int ep_out, ep_in;
};

static uint frequency = 100000;	/* I2C clock frequency in Hz */
module_param(frequency, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(frequency, "I2C clock frequency in hertz");

static struct usb_driver devantech_iss_driver;

/* usb layer */

/* Send command to device, and get response. */
static int devantech_usb_transfer(struct i2c_devantech_iss *dev, int len)
{
	int ret, actual;

	if (len <= 0 || len > ISS_MAX_TRANSFER_LEN)
		return -EINVAL;

	ret = usb_bulk_msg(dev->usb_dev,
			   usb_sndbulkpipe(dev->usb_dev, dev->ep_out),
			   dev->buffer, len, &actual,
			   ISS_USB_TIMEOUT);

	if (!ret) {
		ret = usb_bulk_msg(dev->usb_dev,
				   usb_rcvbulkpipe(dev->usb_dev, dev->ep_in),
				   dev->buffer, sizeof(dev->buffer),
				   &actual, ISS_USB_TIMEOUT);
		if (!ret && actual > 0)
			ret = actual;
	}
	return ret;
}

static int devantech_set_speed(struct i2c_devantech_iss *dev, int speed)
{
	int ret;

	dev->buffer[0] = ISS_CMD_I2C_INTERNAL;
	dev->buffer[1] = ISS_MODE;
	dev->buffer[2] = speed;
	dev->buffer[3] = 0x04;

	ret = devantech_usb_transfer(dev, 4);
	if (ret < 0)
		return ret;
	if (!dev->buffer[0])
		return -EINVAL;
	return 0;
}

static void devantech_fw_version(struct i2c_devantech_iss *dev)
{
	int ret;

	dev->buffer[0] = ISS_CMD_I2C_INTERNAL;
	dev->buffer[1] = ISS_GET_VERSION;

	ret = devantech_usb_transfer(dev, 2);
	if (ret >= 3)
		dev_info(&dev->interface->dev,
			 "Devantech USB-ISS firmware version %u.%u\n",
			 dev->buffer[0], dev->buffer[1]);
	else
		dev_err(&dev->interface->dev,
			"Failed to get firmware version, error %d\n", ret);
}

static void devantech_get_serial(struct i2c_devantech_iss *dev)
{
	int ret;

	dev->buffer[0] = ISS_CMD_I2C_INTERNAL;
	dev->buffer[1] = ISS_GET_SERIAL;

	ret = devantech_usb_transfer(dev, 2);
	if (ret >= 8) {
		dev->buffer[ret] = '\0';
		dev_info(&dev->interface->dev,
			 "Devantech USS-ISS serial number %s\n", dev->buffer);
	} else {
		dev_err(&dev->interface->dev,
			"Failed to get serial number, error %d\n", ret);
	}
}

static int devantech_init(struct i2c_devantech_iss *dev)
{
	int speed;

	/*
	 * Software (bit-bang) I2C implementation does not support I2C clock
	 * stretching, at least not with firmware version 7.2. The PIC hardware
	 * implementation supports it, so select the hardware protocol if
	 * available for a given bus speed.
	 * If the bus speed is not configured or set to 0, select 100kHz.
	 */
	if (frequency >= 1000000) {
		speed = ISS_MODE_I2C_H_1000KHZ;
		frequency = 1000000;
	} else if (frequency >= 400000) {
		speed = ISS_MODE_I2C_H_400KHZ;
		frequency = 400000;
	} else if (frequency >= 100000 || frequency == 0) {
		speed = ISS_MODE_I2C_H_100KHZ;
		frequency = 100000;
	} else if (frequency >= 50000) {
		speed = ISS_MODE_I2C_S_50KHZ;
		frequency = 50000;
	} else {
		speed = ISS_MODE_I2C_S_20KHZ;
		frequency = 20000;
	}

	dev_info(&dev->interface->dev,
		 "Devantech USB-ISS at USB bus %03d address %03d speed %d Hz\n",
		 dev->usb_dev->bus->busnum, dev->usb_dev->devnum, frequency);

	devantech_fw_version(dev);
	devantech_get_serial(dev);

	return devantech_set_speed(dev, speed);
}

/* i2c layer */

static int devantech_usb_xfer(struct i2c_adapter *adapter, struct i2c_msg *msgs,
			      int num)
{
	struct i2c_devantech_iss *dev = i2c_get_adapdata(adapter);
	struct i2c_msg *pmsg = NULL;
	int i, j, len, rlen;
	int ret;

	len = 0;
	dev->buffer[len++] = ISS_CMD_I2C_DIRECT;
	dev->buffer[len++] = ISS_I2C_START;

	for (i = 0; i < num; i++) {
		pmsg = &msgs[i];
		if (i)
			dev->buffer[len++] = ISS_I2C_RESTART;

		if (pmsg->flags & I2C_M_RD) {
			dev->buffer[len++] = ISS_I2C_WRITE(1);
			dev->buffer[len++] = (pmsg->addr << 1) | 1;
			if (pmsg->flags & I2C_M_RECV_LEN) {
				/*
				 * Block read returns receive length in 1st
				 * byte. We must read up to 33 bytes of data
				 * (len + 32) to avoid adapter hangup.
				 */
				dev->buffer[len++] = ISS_I2C_READ(16);
				dev->buffer[len++] = ISS_I2C_READ(16);
				dev->buffer[len++] = ISS_I2C_NACK;
				dev->buffer[len++] = ISS_I2C_READ(1);
			} else {
				rlen = pmsg->len;
				/*
				 * repeat read sequence until done or out of
				 * buffer
				 */
				if (rlen > ISS_MAX_TRANSFER_LEN - 8)
					rlen = ISS_MAX_TRANSFER_LEN - 8;
				while (rlen > 16) {
					dev->buffer[len++] = ISS_I2C_READ(16);
					rlen -= 16;
				}
				if (rlen > 1)
					dev->buffer[len++]
					  = ISS_I2C_READ(rlen - 1);
				dev->buffer[len++] = ISS_I2C_NACK;
				dev->buffer[len++] = ISS_I2C_READ(1);
			}
		} else {
			dev->buffer[len++] = ISS_I2C_WRITE(1);
			dev->buffer[len++] = pmsg->addr << 1;
			/* Repeat write sequence until done */
			for (rlen = 0; rlen < pmsg->len; rlen += 16) {
				int clen = min(pmsg->len - rlen, 16);
				/*
				 * Make sure we have enough buffer space. We
				 * need two additional bytes, one for the write
				 * command itself and one for the STOP command.
				 */
				if (len + clen > ISS_MAX_TRANSFER_LEN - 2) {
					ret = -EINVAL;
					goto error;
				}
				dev->buffer[len++] = ISS_I2C_WRITE(clen);
				for (j = 0; j < clen; j++)
					dev->buffer[len++]
					  = pmsg->buf[rlen + j];
			}
		}
	}
	dev->buffer[len++] = ISS_I2C_STOP;

	ret = devantech_usb_transfer(dev, len);
	if (ret < 0)
		goto error;

	if (ret < 2) {
		ret = -EIO;
		goto error;
	}

	if (dev->buffer[0] == ISS_RESP_NACK) {
		ret = dev->buffer[1] == ISS_RESP_DEV_ERROR ? -ENXIO : -EIO;
		goto error;
	}

	/*
	 * Copy received data back into receive buffer. pmsg points to it.
	 */
	if (pmsg->flags & I2C_M_RD) {
		if (pmsg->flags & I2C_M_RECV_LEN) {
			rlen = dev->buffer[2];
			if (rlen == 0 || rlen > I2C_SMBUS_BLOCK_MAX ||
			    rlen > ret - 2) {
				ret = -EPROTO;
				goto error;
			}
			pmsg->len += rlen;
			len = pmsg->len;
		} else {
			len = ret - 2;
		}
		for (i = 0; i < len; i++)
			pmsg->buf[i] = dev->buffer[i + 2];
	}
	ret = 0;
error:
	return ret;
}

/*
 * Return list of supported functionality.
 */
static u32 devantech_usb_func(struct i2c_adapter *a)
{
	return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL |
	       I2C_FUNC_SMBUS_READ_BLOCK_DATA | I2C_FUNC_SMBUS_BLOCK_PROC_CALL;
}

static const struct i2c_algorithm devantech_usb_algorithm = {
	.master_xfer = devantech_usb_xfer,
	.functionality = devantech_usb_func,
};

/* device layer */

static const struct usb_device_id devantech_iss_table[] = {
	{ USB_DEVICE(USB_VENDOR_ID_DEVANTECH, USB_DEVICE_ID_ISS) },
	{ }
};

MODULE_DEVICE_TABLE(usb, devantech_iss_table);

static void devantech_iss_free(struct i2c_devantech_iss *dev)
{
	usb_put_dev(dev->usb_dev);
	kfree(dev);
}

static int devantech_iss_probe(struct usb_interface *interface,
			       const struct usb_device_id *id)
{
	struct i2c_devantech_iss *dev;
	int ret;
	struct usb_interface *usb_if;
	struct usb_host_endpoint *ep_out, *ep_in;

	/* allocate memory for our device state and initialize it */
	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (dev == NULL) {
		dev_err(&interface->dev, "no memory for device state\n");
		ret = -ENOMEM;
		goto error;
	}

	dev->usb_dev = usb_get_dev(interface_to_usbdev(interface));
	dev->interface = interface;

	/* setup i2c adapter description */
	dev->adapter.owner = THIS_MODULE;
	dev->adapter.class = I2C_CLASS_HWMON;
	dev->adapter.algo = &devantech_usb_algorithm;
	i2c_set_adapdata(&dev->adapter, dev);

	snprintf(dev->adapter.name, sizeof(dev->adapter.name),
		 DRIVER_NAME " at bus %03d device %03d",
		 dev->usb_dev->bus->busnum, dev->usb_dev->devnum);

	dev->adapter.dev.parent = &dev->interface->dev;

	/* Only accept probe on control interface */
	usb_if = usb_ifnum_to_if(dev->usb_dev, 0);
	if (usb_if != interface) {
		dev_err(&interface->dev, "not on control interface\n");
		ret = -ENODEV;
		goto error_free;
	}

	usb_if = usb_ifnum_to_if(dev->usb_dev, 1);
	if (!usb_if) {
		dev_err(&interface->dev, "no USB interface\n");
		ret = -ENODEV;
		goto error_free;
	}
	if (usb_interface_claimed(usb_if)) {
		dev_err(&interface->dev, "USB interface busy\n");
		ret = -EBUSY;
		goto error_free;
	}
	if (usb_if->cur_altsetting->desc.bNumEndpoints < 2) {
		dev_err(&interface->dev,
			"insufficient number of USB endpoints\n");
		ret = -EINVAL;
		goto error_free;
	}

	ep_out = &usb_if->cur_altsetting->endpoint[0];
	ep_in = &usb_if->cur_altsetting->endpoint[1];
	if (!ep_out || !ep_in) {
		dev_err(&interface->dev, "missing USB endpoints\n");
		ret = -EINVAL;
		goto error_free;
	}
	dev->ep_out = ep_out->desc.bEndpointAddress;
	dev->ep_in = ep_in->desc.bEndpointAddress;
	dev->usb_if = usb_get_intf(usb_if);

	/*
	 * We need to claim the data interface to prevent other drivers
	 * from accessing and registering it.
	 */
	ret = usb_driver_claim_interface(&devantech_iss_driver,
					 dev->usb_if, dev);
	if (ret < 0) {
		dev_err(&usb_if->dev, "failed to claim interface\n");
		goto error_put;
	}

	/* save our data pointer in this interface device */
	usb_set_intfdata(interface, dev);

	/* initialize devantech i2c interface */
	ret = devantech_init(dev);
	if (ret < 0) {
		dev_err(&interface->dev, "failed to initialize adapter\n");
		goto error_release;
	}

	/* and finally attach to i2c layer */
	ret = i2c_add_adapter(&dev->adapter);
	if (ret < 0) {
		dev_err(&interface->dev, "failed to add I2C adapter\n");
		goto error_release;
	}

	dev_info(&interface->dev, "connected\n");
	return 0;

error_release:
	usb_set_intfdata(interface, NULL);
	usb_set_intfdata(dev->usb_if, NULL);
	usb_driver_release_interface(&devantech_iss_driver, dev->usb_if);
error_put:
	usb_put_intf(dev->usb_if);
error_free:
	devantech_iss_free(dev);
error:
	return ret;
}

static void devantech_iss_disconnect(struct usb_interface *interface)
{
	struct i2c_devantech_iss *dev = usb_get_intfdata(interface);
	struct usb_host_interface *alt = interface->cur_altsetting;

	if (alt->desc.bInterfaceNumber)
		return;

	i2c_del_adapter(&dev->adapter);
	usb_set_intfdata(interface, NULL);
	usb_set_intfdata(dev->usb_if, NULL);
	usb_driver_release_interface(&devantech_iss_driver, dev->usb_if);
	usb_put_intf(dev->usb_if);
	devantech_iss_free(dev);

	dev_info(&interface->dev, "disconnected\n");
}

static struct usb_driver devantech_iss_driver = {
	.name = DRIVER_NAME,
	.probe = devantech_iss_probe,
	.disconnect = devantech_iss_disconnect,
	.id_table = devantech_iss_table,
};

static int __init devantech_iss_init(void)
{
	/* register this driver with the USB subsystem */
	return usb_register(&devantech_iss_driver);
}

static void __exit devantech_iss_exit(void)
{
	/* deregister this driver with the USB subsystem */
	usb_deregister(&devantech_iss_driver);
}

module_init(devantech_iss_init);
module_exit(devantech_iss_exit);

MODULE_AUTHOR("Guenter Roeck <guenter.roeck@ericsson.com>");
MODULE_DESCRIPTION(DRIVER_NAME " driver");
MODULE_LICENSE("GPL");
