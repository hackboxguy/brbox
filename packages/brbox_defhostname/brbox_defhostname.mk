#############################################################
#
# brbox default-hostname startup-script
#
#############################################################
BRBOX_DEFHOSTNAME_SITE_METHOD = local
BRBOX_DEFHOSTNAME_SITE = $(TOPDIR)/../sources/scripts/cmn/default-hostname
BRBOX_DEFHOSTNAME_INSTALL_STAGING = NO
BRBOX_DEFHOSTNAME_INSTALL_TARGET = YES
BRBOX_DEFHOSTNAME_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

