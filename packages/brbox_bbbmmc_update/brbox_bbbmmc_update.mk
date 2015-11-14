#############################################################
#
# brbox bbbmmc update scripts
#
#############################################################
BRBOX_BBBMMC_UPDATE_SITE_METHOD = local
BRBOX_BBBMMC_UPDATE_SITE = $(TOPDIR)/../sources/scripts/bbbmmc/update
BRBOX_BBBMMC_UPDATE_INSTALL_STAGING = NO
BRBOX_BBBMMC_UPDATE_INSTALL_TARGET = YES
$(eval $(cmake-package))

