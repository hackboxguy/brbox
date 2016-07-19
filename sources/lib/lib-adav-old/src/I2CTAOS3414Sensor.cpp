//this code is based on GroveColorSensor: https://github.com/Seeed-Studio/Grove_I2C_Color_Sensor/blob/master/GroveColorSensor.h
#include "I2CTAOS3414Sensor.hpp"
#include <unistd.h>
#include <string.h>
/*****************************************************************************/
I2CTAOS3414Sensor::I2CTAOS3414Sensor(std::string DevNode,std::string DevType):LightSensor(DevNode)//,I2CBusAccess(DevNode)
{
	sensorI2CAddr=COLOR_SENSOR_ADDR;
	ledStatus = 0;//flashlight led on the grove color sensor
}
I2CTAOS3414Sensor::~I2CTAOS3414Sensor()
{
}
/*****************************************************************************/
void I2CTAOS3414Sensor::setTimingReg()
{
	uint8_t data[16];data[0]=REG_TIMING;data[1]=triggerMode_;
	write_array((uint32_t)sensorI2CAddr, data,2);
	//delay(10); 
}
RPC_SRV_RESULT I2CTAOS3414Sensor::setTimingReg(uint8_t val)
{
	uint8_t data[16];data[0]=REG_TIMING;data[1]=val;
	write_array((uint32_t)sensorI2CAddr, data,2);
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::getTimingReg(uint8_t &val)
{
	uint8_t tmp_data[16];
	uint8_t data[16];data[0]=REG_TIMING_READ;
	write_array((uint32_t)sensorI2CAddr, data,1);
	read_array((uint32_t)sensorI2CAddr, tmp_data,1);
	val=tmp_data[0];
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
void I2CTAOS3414Sensor::setInterruptSourceReg()
{
	uint8_t data[16];data[0]=REG_INT_SOURCE;data[1]=interruptSource_;
	write_array((uint32_t)sensorI2CAddr, data,2);
}
RPC_SRV_RESULT I2CTAOS3414Sensor::setInterruptSourceReg(uint8_t val)
{
	uint8_t data[16];data[0]=REG_INT_SOURCE;data[1]=val;
	write_array((uint32_t)sensorI2CAddr, data,2);
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::getInterruptSourceReg(uint8_t &val)
{
	uint8_t tmp_data[16];
	uint8_t data[16];data[0]=REG_INTRSRC_READ;
	write_array((uint32_t)sensorI2CAddr, data,1);
	read_array((uint32_t)sensorI2CAddr, tmp_data,1);
	val=tmp_data[0];
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
void I2CTAOS3414Sensor::setInterruptControlReg()
{
	uint8_t data[16];data[0]=REG_INT;data[1]=interruptMode_;
	write_array((uint32_t)sensorI2CAddr, data,2);
}
RPC_SRV_RESULT I2CTAOS3414Sensor::setInterruptControlReg(uint8_t val)
{
	uint8_t data[16];data[0]=REG_INT;data[1]=val;
	write_array((uint32_t)sensorI2CAddr, data,2);
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::getInterruptControlReg(uint8_t &val)
{
	uint8_t tmp_data[16];
	uint8_t data[16];data[0]=REG_INTRCTRL_READ;
	write_array((uint32_t)sensorI2CAddr, data,1);
	read_array((uint32_t)sensorI2CAddr, tmp_data,1);
	val=tmp_data[0];
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
void I2CTAOS3414Sensor::setGainReg()
{
	uint8_t data[16];data[0]=REG_GAIN;data[1]=gainAndPrescaler_;
	write_array((uint32_t)sensorI2CAddr, data,2);
}
RPC_SRV_RESULT I2CTAOS3414Sensor::setGainReg(uint8_t val)
{
	uint8_t data[16];data[0]=REG_GAIN;data[1]=val;
	write_array((uint32_t)sensorI2CAddr, data,2);
	return RPC_SRV_RESULT_SUCCESS;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::getGainReg(uint8_t &val)
{
	uint8_t tmp_data[16];
	uint8_t data[16];data[0]=REG_GAIN_READ;
	write_array((uint32_t)sensorI2CAddr, data,1);
	read_array((uint32_t)sensorI2CAddr, tmp_data,1);
	val=tmp_data[0];
	return RPC_SRV_RESULT_SUCCESS;
}
/*****************************************************************************/
void I2CTAOS3414Sensor::setEnableADC()
{
	uint8_t data[16];data[0]=REG_CTL;data[1]=CTL_DAT_INIITIATE;
	write_array((uint32_t)sensorI2CAddr, data,2);
}
/*****************************************************************************/
void I2CTAOS3414Sensor::clearInterrupt()
{
	uint8_t data[16];data[0]=CLR_INT;
	write_array((uint32_t)sensorI2CAddr, data,1);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::readRGB()
{
	uint8_t data[16];data[0]=REG_BLOCK_READ;
	write_array((uint32_t)sensorI2CAddr, data,1);
	
	read_array((uint32_t)sensorI2CAddr, readingdata_,8);
	green_	= readingdata_[1] * 256 + readingdata_[0];
	red_ 	= readingdata_[3] * 256 + readingdata_[2];
	blue_	= readingdata_[5] * 256 + readingdata_[4];
	clear_	= readingdata_[7] * 256 + readingdata_[6];
	
	cout<<"I2CTAOS3414Sensor::readRGB:red   ="<<red_<<endl;
	cout<<"I2CTAOS3414Sensor::readRGB:green ="<<green_<<endl;
	cout<<"I2CTAOS3414Sensor::readRGB:blue  ="<<blue_<<endl;
	cout<<"I2CTAOS3414Sensor::readRGB:white ="<<clear_<<endl;
	return RPC_SRV_RESULT_SUCCESS;
	/*Serial.print("The RGB value are: RGB( ");
	Serial.print(red_,DEC);
	Serial.print(", ");
	Serial.print(green_,DEC);
	Serial.print(", ");
	Serial.print(blue_,DEC);
	Serial.println(" )");
	Serial.print("The Clear channel value are: ");
	Serial.println(clear_,DEC);*/
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::calculateCoordinate()
{
	double X;
	double Y;
	double Z;
	double x;
	double y;
	
	X = (-0.14282) * red_ + (1.54924) * green_ + (-0.95641) * blue_;
	Y = (-0.32466) * red_ + (1.57837) * green_ + (-0.73191) * blue_;
	Z = (-0.68202) * red_ + (0.77073) * green_ + (0.563320) * blue_;
	
	x = X / (X + Y + Z);
	y = Y / (X + Y + Z);
	
	if( (X > 0) && ( Y > 0) && ( Z > 0) )
	{
		//Serial.println("The x,y values are(");
		//Serial.print(x, 2);
		//Serial.print(" , ");
		//Serial.print(y, 2);
		//Serial.println(")");
		return RPC_SRV_RESULT_SUCCESS;
	}
	else
		return RPC_SRV_RESULT_FAIL;//Serial.println("Error: overflow!");
}
/*****************************************************************************/
void I2CTAOS3414Sensor::readRGB(int *red, int *green, int *blue)
{
	uint8_t data[16];data[0]=REG_BLOCK_READ;
	write_array((uint32_t)sensorI2CAddr, data,1);
	read_array((uint32_t)sensorI2CAddr, readingdata_,8);
	green_	= readingdata_[1] * 256 + readingdata_[0];
	red_ 	= readingdata_[3] * 256 + readingdata_[2];
	blue_	= readingdata_[5] * 256 + readingdata_[4];
	clear_	= readingdata_[7] * 256 + readingdata_[6];
	
	double tmp;
	int maxColor;
	
	if ( ledStatus == 1 )
	{
		red_  = red_  * 1.70;
		blue_ = blue_ * 1.35;

		maxColor = max(red_, green_);
		maxColor = max(maxColor, blue_);
	   
		if(maxColor > 255)
		{
			tmp = 250.0/maxColor;
			green_	*= tmp;
			red_ 	*= tmp;
			blue_	*= tmp;
		}
	}
	if ( ledStatus == 0 )
	{
		maxColor = max(red_, green_);
		maxColor = max(maxColor, blue_);
	   
		tmp = 250.0/maxColor;
		green_	*= tmp;
		red_ 	*= tmp;
		blue_	*= tmp;

	}
	
	int minColor = min(red_, green_);
	minColor = min(maxColor, blue_);
	maxColor = max(red_, green_);
	maxColor = max(maxColor, blue_);
	
	int greenTmp = green_;
	int redTmp 	 = red_;
	int blueTmp	 = blue_;
	
	//when turn on LED, need to adjust the RGB data,otherwise it is almost the white color
	if(red_ < 0.8*maxColor && red_ >= 0.6*maxColor)
	{
		red_ *= 0.4;
	}
	else if(red_ < 0.6*maxColor)
	{
		red_ *= 0.2;
	}
	
	if(green_ < 0.8*maxColor && green_ >= 0.6*maxColor)
	{
		green_ *= 0.4;
	}
	else if(green_ < 0.6*maxColor)
	{
		if (maxColor == redTmp && greenTmp >= 2*blueTmp && greenTmp >= 0.2*redTmp)//orange
		{
			green_ *= 5;
		}
		green_ *= 0.2;
	}
	
	if(blue_ < 0.8*maxColor && blue_ >= 0.6*maxColor)
	{
		blue_ *= 0.4;
	}
	else if(blue_ < 0.6*maxColor)
	{
		if (maxColor == redTmp && greenTmp >= 2*blueTmp && greenTmp >= 0.2*redTmp)//orange
		{
			blue_ *= 0.5;
		}
		if (maxColor == redTmp && greenTmp <= blueTmp && blueTmp >= 0.2*redTmp)	//pink
		{
			blue_  *= 5;
		}
		blue_ *= 0.2;
	}
	minColor = min(red_, green_);
	minColor = min(maxColor, blue_);
	if(maxColor == green_ && red_ >= 0.85*maxColor && minColor == blue_)//yellow
	{
		red_ = maxColor;
		blue_ *= 0.4;
	}
	*red   = red_;
	*green = green_;
	*blue  = blue_;
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::init_sensor()
{
	//cout<<"I2CTAOS3414Sensor::init_sensor:called"<<endl;
	triggerMode_     =INTEG_MODE_FREE | INTEG_PARAM_PULSE_COUNT1;
	interruptSource_ =INT_SOURCE_CLEAR;
	interruptMode_   =INTR_LEVEL | INTR_PERSIST_EVERY;
	gainAndPrescaler_=GAIN_1 | PRESCALER_1;
	sensorAddress_   =COLOR_SENSOR_ADDR;
	I2CTAOS3414Sensor::setTimingReg(); 
	I2CTAOS3414Sensor::setInterruptSourceReg();  
	I2CTAOS3414Sensor::setInterruptControlReg(); 
	I2CTAOS3414Sensor::setGainReg(); 
	I2CTAOS3414Sensor::setEnableADC(); 
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::get_integration_time(uint32_t &time)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getTimingReg(val);
	val&=0x0F;//bit-3 to 0
	time=val;
	return ret;//RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::set_integration_time(uint32_t time)
{
	uint8_t val,val1;
	RPC_SRV_RESULT ret=getTimingReg(val);
	if(ret!=RPC_SRV_RESULT_SUCCESS)
		return ret;
	val&=0xF0;//mask bit-3 to 0
	val1=time;val1&=0x0F;
	val|=val1;
	return setTimingReg(val);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::get_sync_edge(ADLIB_STATUS_FLAG_TYPE &edge)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getTimingReg(val);
	val&=0x40;//bit-6
	if(val==0x40)
		edge=ADLIB_STATUS_FLAG_TYPE_HI;
	else
		edge=ADLIB_STATUS_FLAG_TYPE_LO;
	return ret;//RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::set_sync_edge(ADLIB_STATUS_FLAG_TYPE edge)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getTimingReg(val);
	if(ret!=RPC_SRV_RESULT_SUCCESS)
		return ret;
	val&=0xBF;//bit-6
	if(edge==ADLIB_STATUS_FLAG_TYPE_HI)
		val|=0x40;//make bit-6 high
	//else : bit is already made low 
		//val&=0xBF;//make bit-6 low
	return setTimingReg(val);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::get_integration_mode(LS_INTEG_MODE &mode)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getTimingReg(val);
	val&=0x30;//bit-5 and bit-4
	val>>=4;
	switch(val)
	{
		case 0x00:mode=LS_INTEG_MODE_FREE;break;
		case 0x01:mode=LS_INTEG_MODE_MANUAL;break;
		case 0x02:mode=LS_INTEG_MODE_SYNC_SINGLE;break;
		case 0x03:mode=LS_INTEG_MODE_SYNC_MULTI;break;
		default:mode=LS_INTEG_MODE_UNKNOWN;break;
	}
	return ret;//RPC_SRV_RESULT_FEATURE_NOT_AVAILABLE;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::set_integration_mode(LS_INTEG_MODE mode)
{
	uint8_t val,val1;
	RPC_SRV_RESULT ret=getTimingReg(val);
	if(ret!=RPC_SRV_RESULT_SUCCESS)
		return ret;
	val&=0xCF;//bit-5 and bit-4
	val1=mode&0x03;val1<<=4;
	val|=val1;
	return setTimingReg(val);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::get_intr_stop_sts(ADLIB_STATUS_FLAG_TYPE &sts)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getInterruptControlReg(val);
	val&=0x40;//bit-6
	if(val==0x40)
		sts=ADLIB_STATUS_FLAG_TYPE_HI;
	else
		sts=ADLIB_STATUS_FLAG_TYPE_LO;
	return ret;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::set_intr_stop_sts(ADLIB_STATUS_FLAG_TYPE sts)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getInterruptControlReg(val);
	if(ret!=RPC_SRV_RESULT_SUCCESS)
		return ret;
	val&=0xBF;//bit-6
	if(sts==ADLIB_STATUS_FLAG_TYPE_HI)
		val|=0x40;//make bit-6 high
	//else: bit already made low
	return setInterruptControlReg(val);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::get_intr_mode(LS_INTR_MODE &mode)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getInterruptControlReg(val);
	val&=0x30;//bit-5 and bit-4
	val>>=4;
	switch(val)
	{
		case 0x00:mode=LS_INTR_MODE_DISABLE;break;
		case 0x01:mode=LS_INTR_MODE_LEVEL;break;
		case 0x02:mode=LS_INTR_MODE_SMB_ALERT;break;
		case 0x03:mode=LS_INTR_MODE_SMB_ALERT_SIMULATE;break;
		default:mode=LS_INTR_MODE_UNKNOWN;break;
	}
	return ret;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::set_intr_mode(LS_INTR_MODE mode)
{
	uint8_t val,val1;
	RPC_SRV_RESULT ret=getInterruptControlReg(val);
	if(ret!=RPC_SRV_RESULT_SUCCESS)
		return ret;
	val&=0xCF;//bit-5 and bit-4
	val1=mode&0x03;val1<<=4;
	val|=val1;
	return setInterruptControlReg(val);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::get_intr_rate(LS_INTR_RATE &rate)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getInterruptControlReg(val);
	val&=0x07;//bit-2 to bit-0
	//val>>=4;
	switch(val)
	{
		case 0x00:rate=LS_INTR_RATE_EVERY;break;
		case 0x01:rate=LS_INTR_RATE_SINGLE;break;
		case 0x02:rate=LS_INTR_RATE_100MS;break;
		case 0x03:rate=LS_INTR_RATE_SECOND;break;
		default:rate=LS_INTR_RATE_UNKNOWN;break;
	}
	return ret;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::set_intr_rate(LS_INTR_RATE rate)
{
	uint8_t val,val1;
	RPC_SRV_RESULT ret=getInterruptControlReg(val);
	if(ret!=RPC_SRV_RESULT_SUCCESS)
		return ret;
	val&=0xF8;//bit-5 and bit-4
	val1=rate&0x07;
	val|=val1;
	return setInterruptControlReg(val);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::get_intr_source(LS_INTR_SOURCE &src)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getInterruptSourceReg(val);
	val&=0x03;//bit-1 to bit-0
	//val>>=4;
	switch(val)
	{
		case 0x00:src=LS_INTR_SOURCE_GREEN;break;
		case 0x01:src=LS_INTR_SOURCE_RED;break;
		case 0x02:src=LS_INTR_SOURCE_BLUE;break;
		case 0x03:src=LS_INTR_SOURCE_WHITE;break;
		default:src=LS_INTR_SOURCE_UNKNOWN;break;
	}
	return ret;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::set_intr_source(LS_INTR_SOURCE src)
{
	uint8_t val;//,val1;
	//RPC_SRV_RESULT ret=getInterruptSourceReg(val);
	//if(ret!=RPC_SRV_RESULT_SUCCESS)
	//	return ret;
	//val&=0xF8;//bit-5 and bit-4
	//val1=rate&0x07;
	val=(uint8_t)src&0x03;
	return setInterruptSourceReg(val);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::get_gain_mode(LS_GAIN_MODE &mode)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getGainReg(val);
	val&=0x30;//bit-5 and bit-4
	val>>=4;
	switch(val)
	{
		case 0x00:mode=LS_GAIN_MODE_1;break;
		case 0x01:mode=LS_GAIN_MODE_4;break;
		case 0x02:mode=LS_GAIN_MODE_16;break;
		case 0x03:mode=LS_GAIN_MODE_64;break;
		default:mode=LS_GAIN_MODE_UNKNOWN;break;
	}
	return ret;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::set_gain_mode(LS_GAIN_MODE mode)
{
	uint8_t val,val1;
	RPC_SRV_RESULT ret=getGainReg(val);
	if(ret!=RPC_SRV_RESULT_SUCCESS)
		return ret;
	val&=0xCF;//bit-5 and bit-4
	val1=mode&0x03;val1<<=4;
	val|=val1;
	return setGainReg(val);
}
/*****************************************************************************/
RPC_SRV_RESULT I2CTAOS3414Sensor::get_prescaler(LS_PRESCALER &mode)
{
	uint8_t val;
	RPC_SRV_RESULT ret=getGainReg(val);
	val&=0x07;//bit-2 to bit-0
	switch(val)
	{
		case 0x00:mode=LS_PRESCALER_DIV1;break;
		case 0x01:mode=LS_PRESCALER_DIV2;break;
		case 0x02:mode=LS_PRESCALER_DIV4;break;
		case 0x03:mode=LS_PRESCALER_DIV8;break;
		case 0x04:mode=LS_PRESCALER_DIV16;break;
		case 0x05:mode=LS_PRESCALER_DIV32;break;
		case 0x06:mode=LS_PRESCALER_DIV64;break;
		default:mode=LS_PRESCALER_UNKNOWN;break;
	}
	return ret;
}
RPC_SRV_RESULT I2CTAOS3414Sensor::set_prescaler(LS_PRESCALER mode)
{
	uint8_t val,val1;
	RPC_SRV_RESULT ret=getGainReg(val);
	if(ret!=RPC_SRV_RESULT_SUCCESS)
		return ret;
	val&=0xF8;
	val1=(mode&0x07);
	val|=val1;
	return setGainReg(val);
}
/*****************************************************************************/
//get/set sync_edge(enum hi/low)
//get/set integ_mode()
//get/set integ_time(0-to-16 count value).

