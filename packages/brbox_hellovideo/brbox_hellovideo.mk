#############################################################
#
# hellovideo (rpi video player)
#
#############################################################
BRBOX_HELLOVIDEO_VERSION = 0.1
BRBOX_HELLOVIDEO_SITE_METHOD = local
BRBOX_HELLOVIDEO_SITE = $(TOPDIR)/../sources/utils/hello_video
BRBOX_HELLOVIDEO_INSTALL_STAGING = NO
BRBOX_HELLOVIDEO_INSTALL_TARGET = YES
BRBOX_HELLOVIDEO_DEPENDENCIES = brbox_libIlClient
BRBOX_HELLOVIDEO_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF
$(eval $(cmake-package))

