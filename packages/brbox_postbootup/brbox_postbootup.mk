#############################################################
#
# brbox post-bootup(late-init) script for custom activities.
#
#############################################################
BRBOX_POSTBOOTUP_SITE_METHOD = local
BRBOX_POSTBOOTUP_SITE = $(TOPDIR)/../sources/scripts/postbootup
BRBOX_POSTBOOTUP_INSTALL_STAGING = NO
BRBOX_POSTBOOTUP_INSTALL_TARGET = YES
BRBOX_POSTBOOTUP_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

