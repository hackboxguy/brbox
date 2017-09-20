#############################################################
#
# brbox gphoto2 commandline utility
#
#############################################################
BRBOX_GPHOTO2_VERSION = master 
#BRBOX_GPHOTO2_SITE_METHOD = local
BRBOX_GPHOTO2_SITE = $(call github,gphoto,gphoto2,$(BRBOX_GPHOTO2_VERSION))
#BRBOX_GPHOTO2_SITE = $(TOPDIR)/../prebuilt-bins/raspi1
#BRBOX_GPHOTO2_INSTALL_STAGING = YES #NO
#BRBOX_GPHOTO2_INSTALL_TARGET = YES
BRBOX_GPHOTO2_DEPENDENCIES = gettext popt host-pkgconf libgphoto2
BRBOX_GPHOTO2_LICENSE = GPL-2.0
BRBOX_GPHOTO2_LICENSE_FILES = COPYING
BRBOX_GPHOTO2_GETTEXTIZE = YES
BRBOX_GPHOTO2_AUTORECONF = YES

#BRBOX_GPHOTO2_AUTORECONF_OPTS = --install --symlink
#BRBOX_GPHOTO2_CONF_OPTS = --install --symlink
#HOST_BRBOX_GPHOTO2_CONF_OPTS = --install --symlink

#define BRBOX_GPHOTO2_CREATE_M4_DIR
#	ln -s $(@D)/gphoto-m4 $(@D)/auto-m4
#endef

#BRBOX_GPHOTO2_POST_PATCH_HOOKS += BRBOX_GPHOTO2_CREATE_M4_DIR


#define BRBOX_GPHOTO2_BOOTSTRAP
#	cd $(@D) && PATH=$(BR_PATH) AUTOPOINT=/bin/true 
#endef
#BRBOX_GPHOTO2_PRE_CONFIGURE_HOOKS += BRBOX_GPHOTO2_BOOTSTRAP


$(eval $(autotools-package))
$(eval $(host-autotools-package))
