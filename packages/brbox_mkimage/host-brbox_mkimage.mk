#############################################################
#
# mkimage for host
#
#############################################################
BRBOX_MKIMAGE_VERSION = 0.1
BRBOX_MKIMAGE_SITE_METHOD = local
BRBOX_MKIMAGE_SITE = $(TOPDIR)/../sources/utils/brbox-mkimage
BRBOX_MKIMAGE_INSTALL_STAGING = NO
BRBOX_MKIMAGE_INSTALL_TARGET = NO
HOST_BRBOX_MKIMAGE_CONF_OPTS=-DAUTO_SVN_VERSION=OFF #todo: when SITE_METHOD is svnco, enable this flag
#BRBOX_MKIMAGE_CONF_OPT=-DCMAKE_INSTALL_PREFIX="opt/bin"
$(eval $(host-cmake-package))

