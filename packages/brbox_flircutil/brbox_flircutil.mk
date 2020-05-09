################################################################################
#
# flirc utility
#
################################################################################
BRBOX_FLIRCUTIL_VERSION = master
BRBOX_FLIRCUTIL_SITE = $(call github,hackboxguy,flirc-sdk,$(BRBOX_FLIRC_VERSION))
BRBOX_FLIRCUTIL_INSTALL_STAGING = NO
BRBOX_FLIRCUTIL_DEPENDENCIES = readline hidapi libusb
BRBOX_FLIRCUTIL_LICENSE = GPL-2.0
BRBOX_FLIRCUTIL_LICENSE_FILES = COPYING

define BRBOX_FLIRCUTIL_BUILD_CMDS
	$(MAKE) all -C $(@D)
endef

define BRBOX_FLIRCUTIL_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/cli/buildresults/*/*/flirc_util/release/flirc_util $(TARGET_DIR)/usr/bin/flirc_util
endef

$(eval $(generic-package))
