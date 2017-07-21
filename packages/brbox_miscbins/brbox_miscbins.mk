#############################################################
#
# brbox misc-binary images(ex: patterns etc)
#
#############################################################
BRBOX_MISCBINS_SITE_METHOD = local
BRBOX_MISCBINS_SITE = $(TOPDIR)/../sources/misc-binaries
BRBOX_MISCBINS_INSTALL_STAGING = NO
BRBOX_MISCBINS_INSTALL_TARGET = YES
#BRBOX_MISCBINS_DEPENDENCIES = tftpd
BRBOX_MISCBINS_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

