################################################################################
#
# brbox_fbv_improved
#
################################################################################
BRBOX_FIM_VERSION = 0.2.2
BRBOX_FIM_SOURCE = fim-$(DMIDECODE_VERSION).tar.bz2
BRBOX_FIM_SITE = http://download.savannah.gnu.org/releases/fim
BRBOX_FIM_LICENSE = GPL-2.0+
BRBOX_FIM_LICENSE_FILES = LICENSE

define BRBOX_FIM_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D) $(TARGET_CONFIGURE_OPTS)
endef

define BRBOX_FIM_INSTALL_TARGET_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D) prefix=/usr DESTDIR=$(TARGET_DIR) install
endef

$(eval $(generic-package))
