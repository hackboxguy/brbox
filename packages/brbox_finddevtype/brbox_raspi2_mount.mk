#############################################################
#
# brbox find-devtype startup-script
#
#############################################################
BRBOX_FINDDEVTYPE_SITE_METHOD = local
BRBOX_FINDDEVTYPE_SITE = $(TOPDIR)/../sources/scripts/cmn/find-devtype
BRBOX_FINDDEVTYPE_INSTALL_STAGING = NO
BRBOX_FINDDEVTYPE_INSTALL_TARGET = YES
BRBOX_FINDDEVTYPE_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

