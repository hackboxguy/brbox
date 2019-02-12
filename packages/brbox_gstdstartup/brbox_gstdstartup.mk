#############################################################
#
# brbox gstd-daemon startup-script
#
#############################################################
BRBOX_GSTDSTARTUP_SITE_METHOD = local
BRBOX_GSTDSTARTUP_SITE = $(TOPDIR)/../sources/scripts/cmn/gstd-startup-script
BRBOX_GSTDSTARTUP_INSTALL_STAGING = NO
BRBOX_GSTDSTARTUP_INSTALL_TARGET = YES
BRBOX_GSTDSTARTUP_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

