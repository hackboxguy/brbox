#############################################################
#
# brbox lightyconfig overlay
#
#############################################################
BRBOX_LIGHTYCONFIG_SITE_METHOD = local
BRBOX_LIGHTYCONFIG_SITE = $(TOPDIR)/../sources/scripts/cmn/lighty-config
BRBOX_LIGHTYCONFIG_INSTALL_STAGING = NO
BRBOX_LIGHTYCONFIG_INSTALL_TARGET = YES
BRBOX_LIGHTYCONFIG_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

