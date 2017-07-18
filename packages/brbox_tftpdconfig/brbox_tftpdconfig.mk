#############################################################
#
# brbox tftpd-config overlay
#
#############################################################
BRBOX_TFTPDCONFIG_SITE_METHOD = local
BRBOX_TFTPDCONFIG_SITE = $(TOPDIR)/../sources/scripts/cmn/tftpd-config
BRBOX_TFTPDCONFIG_INSTALL_STAGING = NO
BRBOX_TFTPDCONFIG_INSTALL_TARGET = YES
BRBOX_TFTPDCONFIG_DEPENDENCIES = tftpd
BRBOX_TFTPDCONFIG_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

