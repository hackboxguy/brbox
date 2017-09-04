#############################################################
#
# brbox prepare-boot-disk-script.
#
#############################################################
BRBOX_PREPAREBOOTDISK_SITE_METHOD = local
BRBOX_PREPAREBOOTDISK_SITE = $(TOPDIR)/../sources/scripts/cmn/prepare-boot-disk
BRBOX_PREPAREBOOTDISK_INSTALL_STAGING = NO
BRBOX_PREPAREBOOTDISK_INSTALL_TARGET = YES
BRBOX_PREPAREBOOTDISK_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

