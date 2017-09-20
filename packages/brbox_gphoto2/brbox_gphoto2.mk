#############################################################
#
# brbox gphoto2 commandline utility
#
#############################################################
BRBOX_GPHOTO2_VERSION = master 
#BRBOX_GPHOTO2_SITE_METHOD = local
BRBOX_GPHOTO2_SITE = $(call github,gphoto,gphoto2,$(BRBOX_GPHOTO2_VERSION))
#BRBOX_GPHOTO2_SITE = $(TOPDIR)/../prebuilt-bins/raspi1
BRBOX_GPHOTO2_INSTALL_STAGING = NO
#BRBOX_GPHOTO2_INSTALL_TARGET = YES
BRBOX_GPHOTO2_DEPENDENCIES = host-pkgconf libgphoto2
BRBOX_GPHOTO2_LICENSE = GPL-2.0
BRBOX_GPHOTO2_LICENSE_FILES = COPYING
BRBOX_GPHOTO2_AUTORECONF = YES
BRBOX_GPHOTO2_CONF_OPTS = --install --symlink

#define BRBOX_GPHOTO2_CREATE_M4_DIR
 #       mkdir -p $(@D)/auto-m4
#endef

#BRBOX_GPHOTO2_POST_PATCH_HOOKS += BRBOX_GPHOTO2_CREATE_M4_DIR

$(eval $(autotools-package))
$(eval $(host-autotools-package))
