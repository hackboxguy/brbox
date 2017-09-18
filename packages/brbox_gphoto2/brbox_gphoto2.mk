#############################################################
#
# brbox gphoto2 commandline utility
#
#############################################################
BRBOX_GPHOTO2_VERSION = 2_4
#BRBOX_GPHOTO2_SITE_METHOD = local
BRBOX_GPHOTO2_SITE = $(call github,gphoto,gphoto2,$(BRBOX_GPHOTO2_VERSION))
#BRBOX_GPHOTO2_SITE = $(TOPDIR)/../prebuilt-bins/raspi1
BRBOX_GPHOTO2_INSTALL_STAGING = NO
BRBOX_GPHOTO2_INSTALL_TARGET = YES
BRBOX_GPHOTO2_DEPENDENCIES = libgphoto2
BRBOX_GPHOTO2_LICENSE = GPL-2.0
BRBOX_GPHOTO2_LICENSE_FILES = COPYING
#BRBOX_ZBARIMG_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(generic-package))

