#############################################################
#
# brbox raspi2 mount startup-script
#
#############################################################
BRBOX_RASPI2_MOUNT_SITE_METHOD = local
BRBOX_RASPI2_MOUNT_SITE = $(TOPDIR)/../sources/scripts/raspi2/boot
BRBOX_RASPI2_MOUNT_INSTALL_STAGING = NO
BRBOX_RASPI2_MOUNT_INSTALL_TARGET = YES
BRBOX_RASPI2_MOUNT_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

