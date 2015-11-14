#############################################################
#
# brbox bbbmmc mount startup-script
#
#############################################################
BRBOX_BBBMMC_MOUNT_SITE_METHOD = local
BRBOX_BBBMMC_MOUNT_SITE = $(TOPDIR)/../sources/scripts/bbbmmc/boot
BRBOX_BBBMMC_MOUNT_INSTALL_STAGING = NO
BRBOX_BBBMMC_MOUNT_INSTALL_TARGET = YES
BRBOX_BBBMMC_MOUNT_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

