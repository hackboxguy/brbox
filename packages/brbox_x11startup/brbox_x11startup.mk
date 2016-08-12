#############################################################
#
# brbox x display-server startup-script
#
#############################################################
BRBOX_X11STARTUP_SITE_METHOD = local
BRBOX_X11STARTUP_SITE = $(TOPDIR)/../sources/scripts/cmn/X11-startup-script
BRBOX_X11STARTUP_INSTALL_STAGING = NO
BRBOX_X11STARTUP_INSTALL_TARGET = YES
BRBOX_X11STARTUP_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

