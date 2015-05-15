#############################################################
#
# brbox baytrail update scripts
#
#############################################################
BRBOX_BAYTRAIL_UPDATE_SITE_METHOD = local
BRBOX_BAYTRAIL_UPDATE_SITE = $(TOPDIR)/../sources/scripts/baytrail_update
BRBOX_BAYTRAIL_UPDATE_INSTALL_STAGING = NO
BRBOX_BAYTRAIL_UPDATE_INSTALL_TARGET = YES
$(eval $(cmake-package))

