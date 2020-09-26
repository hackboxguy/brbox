################################################################################
#
# actkbd daemon
#
################################################################################
BRBOX_ACTKBD_VERSION = master
BRBOX_ACTKBD_SITE = $(call github,thkala,actkbd,$(BRBOX_ACTKBD_VERSION))
BRBOX_ACTKBD_INSTALL_STAGING = NO
BRBOX_ACTKBD_DEPENDENCIES = readline hidapi
BRBOX_ACTKBD_LICENSE = GPL-2.0
BRBOX_ACTKBD_LICENSE_FILES = COPYING

define BRBOX_ACTKBD_BUILD_CMDS
	$(MAKE) all -C $(@D)
endef

define BRBOX_ACTKBD_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/actkbd $(TARGET_DIR)/usr/bin/actkbd
endef

$(eval $(generic-package))
