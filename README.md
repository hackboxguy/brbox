
# brbox

**brbox** is a bare-metal Linux system built with Buildroot. It provides the foundational features of an embedded Linux system, serving as a base for extending and integrating various applications. 

### Key Features
1. **Failsafe In-System Programmability**: Allows network-based Linux system updates using backup and main rootfs copies(A/B).
2. **Zeroconf Announcement**: Makes it easy to detect the IP of headless embedded Linux systems running brbox.
3. **System Monitoring**: Displays parameters such as IP, MAC address, RAM size, and CPU load via the web interface.

---

## Setting Up for brbox on ubuntu machine
1. Create and prepare a build directory:
   ```bash
   sudo mkdir -p /mnt/buildramdisk
   sudo chown user:user /mnt/buildramdisk  # Replace 'user' with your username
   ```
2. Install required dependencies:
   ```bash
   sudo apt-get update
   sudo apt-get install subversion git ckermit build-essential gcc-multilib libncurses5-dev expect
   ```
3. Configure temporary RAM disk(optional - but this step helps not to stress your hdd due to large amount of buid data):
   ```bash
   sudo vi /etc/fstab
   ```
   Add the following line(change the size as per your hw config):
   ```
   tmpfs   /mnt/buildramdisk    tmpfs   defaults,gid=100,size=6G 0 0
   ```

---

## How to build brbox images for different hardwares
1. Prepare directories:
   ```bash
   mkdir -p /home/user/dl
   mkdir -p /home/user/sandbox
   cd /home/user/sandbox
   ```
2. Clone the repository:
   ```bash
   svn co http://github.com/hackboxguy/brbox
   cd brbox/trunk
   ln -s /home/user/dl dl
   ```
3. Prepare and build the system(e.g for raspberry-pi-0):
   ```bash
   ./prepare-buildroot.sh
   ./build-image.sh -o /mnt/buildramdisk -v 01.00 -s mypw -b raspi0
   ```
   - `-o`: Output folder  
   - `-v`: Build version number  
   - `-s`: Your sudo password  
   - `-s`: [board type](https://github.com/hackboxguy/brbox/blob/a30dc0581392fada006db40733abf6883c28bf12/build-image.sh#L30)

The first build may take 1–2 hours. Once complete, the following files will be generated:
- **bootable-usb-disk.img**: Use this image for creating a bootable USB drive.
- **uBrBoxRoot.uimg**: Upgrade package for in-system updates(OTA).

---

## Creating a Bootable USB Flash Drive
1. Use the `dd` command to write the image to a USB drive:
   ```bash
   sudo dd if=/mnt/buildramdisk/01.00/images/bootable-usb-disk.img of=/dev/sdX bs=1M
   ```
   Replace `/dev/sdX` with the correct device node.
2. Configure your BIOS to boot from the USB drive and disable EFI boot.
3. Insert the USB drive and power up your system. Log in using the `root` account.

---

## Upgrading brbox over Network(OTA)
1. Copy the upgrade package to the device:
   ```bash
   scp /mnt/buildramdisk/01.00/images/uBrBoxRoot.uimg root@<brbox-ip>:/tmp/
   ```
2. Log in via SSH:
   ```bash
   ssh root@<brbox-ip>
   ```
3. Perform the upgrade:
   ```bash
   brdskmgr -v  # Check current version
   update -u /tmp/uBrBoxRoot.uimg
   reboot
   ```
4. Verify the upgrade:
   ```bash
   ssh root@<brbox-ip>
   brdskmgr -v  # Confirm the updated version
   ```

---

## Building brbox for Raspberry Pi 1
1. Create and build the image:
   ```bash
   cd /home/user/sandbox/
   svn co https://github.com/hackboxguy/brbox
   cd brbox/trunk
   ln -s /home/user/dl dl
   ./prepare-buildroot.sh
   time ./build-image.sh -o /mnt/buildramdisk -v 00.01 -s my_sudo_pw -b raspi1
   ```
2. Create a bootable SD card:
   ```bash
   sudo sh -c "pv -tpreb /mnt/buildramdisk/00.01/images/bootable-usb-disk.img | dd bs=128k of=/dev/sdX conv=fdatasync"
   ```
   Replace `/dev/sdX` with the correct device node.
3. Boot your Raspberry Pi with the SD card and log in as `root` with the password `brb0x`.

---

## Cross-Compiling for Raspberry Pi 1 on x86 Host
1. Set up the toolchain:
   ```bash
   cd /home/user/sandbox/
   svn co https://github.com/hackboxguy/downloads
   cp downloads/toolchain/brbox-raspi1-disptst.tar.xz /opt
   cd /opt/
   tar -xvf brbox-raspi1-disptst.tar.xz
   ln -s brbox-raspi1-disptst/ brbox-raspi1-arm
   ```
2. Compile the source code:
   ```bash
   cd /home/user/sandbox/
   svn co https://github.com/hackboxguy/brbox
   cd brbox/trunk/sources
   cmake -H. -BOutput -DCMAKE_INSTALL_PREFIX=/home/usr/tmp/brbox-install -DCMAKE_TOOLCHAIN_FILE=cmake/brbox-raspi1-arm.cmake
   cmake --build Output -- install -j4
   ```
3. Transfer binaries to the Raspberry Pi:
   ```bash
   scp -r /home/usr/tmp/brbox-install <raspi-ip>:/path/to/target
   ```

---

