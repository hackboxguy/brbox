#############################################################
#
# brbox umtskeeper for 3g data connectivity
#
#############################################################
BRBOX_UMTSKEEPER_SITE_METHOD = local
BRBOX_UMTSKEEPER_SITE = $(TOPDIR)/../sources/scripts/cmn/umts-keeper
BRBOX_UMTSKEEPER_INSTALL_STAGING = NO
BRBOX_UMTSKEEPER_INSTALL_TARGET = YES
BRBOX_UMTSKEEPER_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

