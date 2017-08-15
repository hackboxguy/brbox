################################################################################
#
# fb-test-app
#
################################################################################

BRBOX_FBTESTAPP_VERSION = rosetta-1.1.0
BRBOX_FBTESTAPP_SITE_METHOD = local
BRBOX_FBTESTAPP_SITE = $(TOPDIR)/../sources/utils/fb-test-app
BRBOX_FBTESTAPP_LICENSE = GPL-2.0
BRBOX_FBTESTAPP_LICENSE_FILES = COPYING

define BRBOX_FBTESTAPP_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D) all
endef

define BRBOX_FBTESTAPP_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/perf $(TARGET_DIR)/usr/bin/fb-test-perf
	$(INSTALL) -D -m 0755 $(@D)/rect $(TARGET_DIR)/usr/bin/fb-test-rect
	$(INSTALL) -D -m 0755 $(@D)/fb-test $(TARGET_DIR)/usr/bin/fb-test
	$(INSTALL) -D -m 0755 $(@D)/offset $(TARGET_DIR)/usr/bin/fb-test-offset
endef

$(eval $(generic-package))
