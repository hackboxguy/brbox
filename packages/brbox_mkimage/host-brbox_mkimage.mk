#############################################################
#
# mkimage for host
#
#############################################################
BRBOX_MKIMAGE_SITE_METHOD = local
BRBOX_MKIMAGE_SITE = $(TOPDIR)/../sources/utils/brbox-mkimage
BRBOX_MKIMAGE_INSTALL_STAGING = NO
BRBOX_MKIMAGE_INSTALL_TARGET = NO
BRBOX_MKIMAGE_CONF_OPT = -DMKIMG_HOST_BUILD=ON
$(eval $(host-cmake-package))

