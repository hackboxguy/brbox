#############################################################
#
# brbox bbb mount startup-script
#
#############################################################
BRBOX_BBB_MOUNT_SITE_METHOD = local
BRBOX_BBB_MOUNT_SITE = $(TOPDIR)/../sources/scripts/bbb/boot
BRBOX_BBB_MOUNT_INSTALL_STAGING = NO
BRBOX_BBB_MOUNT_INSTALL_TARGET = YES
BRBOX_BBB_MOUNT_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/etc/"
$(eval $(cmake-package))

