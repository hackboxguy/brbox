#############################################################
#
# brbox raspi2 update scripts
#
#############################################################
BRBOX_RASPI2_UPDATE_SITE_METHOD = local
BRBOX_RASPI2_UPDATE_SITE = $(TOPDIR)/../sources/scripts/raspi2/update
BRBOX_RASPI2_UPDATE_INSTALL_STAGING = NO
BRBOX_RASPI2_UPDATE_INSTALL_TARGET = YES
$(eval $(cmake-package))

