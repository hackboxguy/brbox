#############################################################
#
# brbox zbarimg prebuilt binary
#
#############################################################
BRBOX_ZBARIMG_SITE_METHOD = local
BRBOX_ZBARIMG_SITE = $(TOPDIR)/../prebuilt-bins/raspi1
BRBOX_ZBARIMG_INSTALL_STAGING = NO
BRBOX_ZBARIMG_INSTALL_TARGET = YES
#BRBOX_ZBARIMG_DEPENDENCIES = lighttpd
BRBOX_ZBARIMG_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

