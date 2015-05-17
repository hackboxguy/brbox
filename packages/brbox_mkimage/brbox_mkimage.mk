#############################################################
#
# mkimage for target
#
#############################################################
BRBOX_MKIMAGE_VERSION = 0.1
BRBOX_MKIMAGE_SITE_METHOD = local
BRBOX_MKIMAGE_SITE = $(TOPDIR)/../sources/utils/brbox-mkimage
BRBOX_MKIMAGE_INSTALL_STAGING = NO
BRBOX_MKIMAGE_INSTALL_TARGET = YES
BRBOX_MKIMAGE_CONF_OPTS=-DMKIMG_HOST_BUILD=OFF -DAUTO_SVN_VERSION=OFF #todo: when SITE_METHOD is svnco, enable AUTO_SVN_VERSION flag
#BRBOX_MKIMAGE_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/"
$(eval $(cmake-package))

