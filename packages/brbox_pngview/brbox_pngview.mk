################################################################################
#
# pngview-app
#
################################################################################
BRBOX_PNGVIEW_VERSION = master
#BRBOX_PNGVIEW_SITE_METHOD = local
#BRBOX_PNGVIEW_SITE = $(TOPDIR)/../sources/utils/pngview
BRBOX_PNGVIEW_SITE = $(call github,AndrewFromMelbourne,raspidmx,$(BRBOX_PNGVIEW_VERSION))
BRBOX_PNGVIEW_INSTALL_STAGING = NO
BRBOX_PNGVIEW_LICENSE = GPL-2.0
BRBOX_PNGVIEW_LICENSE_FILES = COPYING

#define BRBOX_PNGVIEW_BUILD_CMDS
#	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D) all
#endef

#define BRBOX_PNGVIEW_INSTALL_TARGET_CMDS
#	$(INSTALL) -D -m 0755 $(@D)/pngview $(TARGET_DIR)/usr/bin/pngview
#endef

$(eval $(generic-package))
