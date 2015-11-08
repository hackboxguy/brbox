#############################################################
#
# brbox bbb update scripts
#
#############################################################
BRBOX_BBB_UPDATE_SITE_METHOD = local
BRBOX_BBB_UPDATE_SITE = $(TOPDIR)/../sources/scripts/bbb/update
BRBOX_BBB_UPDATE_INSTALL_STAGING = NO
BRBOX_BBB_UPDATE_INSTALL_TARGET = YES
$(eval $(cmake-package))

