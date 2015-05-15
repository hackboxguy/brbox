#############################################################
#
# mkimage for target
#
#############################################################
BRBOX_MKIMAGE_SITE_METHOD = local
BRBOX_MKIMAGE_SITE = $(TOPDIR)/../sources/utils/brbox-mkimage
BRBOX_MKIMAGE_INSTALL_STAGING = NO
BRBOX_MKIMAGE_INSTALL_TARGET = YES
$(eval $(cmake-package))

