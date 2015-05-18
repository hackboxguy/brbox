#############################################################
#
# brbox baytrail mount startup-script
#
#############################################################
BRBOX_BAYTRAIL_MOUNT_SITE_METHOD = local
BRBOX_BAYTRAIL_MOUNT_SITE = $(TOPDIR)/../sources/scripts/baytrail/boot
BRBOX_BAYTRAIL_MOUNT_INSTALL_STAGING = NO
BRBOX_BAYTRAIL_MOUNT_INSTALL_TARGET = YES
BRBOX_BAYTRAIL_MOUNT_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/etc/"
$(eval $(cmake-package))

