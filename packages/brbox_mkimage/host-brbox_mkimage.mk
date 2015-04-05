#############################################################
#
# modified mkimage for BRBOX use
#
#############################################################

HOST_BRBOX_MKIMAGE_DIR:=$(BUILD_DIR)/host-brbox_mkimage
HOST_BRBOX_MKIMAGE_BIN:=$(HOST_BRBOX_MKIMAGE_DIR)/build

# Strip off the annoying quoting
HOST_BRBOX_MKIMAGE_SRC:=$(call qstrip,$(BR2_PACKAGE_BRBOX_MKIMAGE_SOURCE))

$(HOST_BRBOX_MKIMAGE_DIR)/.stamp_downloaded:
	mkdir -p $(BUILD_DIR)
	svn co $(HOST_BRBOX_MKIMAGE_SRC) $(HOST_BRBOX_MKIMAGE_DIR)
	touch $@

$(HOST_BRBOX_MKIMAGE_DIR)/.stamp_extracted: $(HOST_BRBOX_MKIMAGE_DIR)/.stamp_downloaded
	touch $@

$(HOST_BRBOX_MKIMAGE_DIR)/.stamp_configured: $(HOST_BRBOX_MKIMAGE_DIR)/.stamp_extracted
	touch $@

$(HOST_BRBOX_MKIMAGE_DIR)/.stamp_built: $(HOST_BRBOX_MKIMAGE_DIR)/.stamp_configured
	$(HOST_CONFIGURE_OPTS) $(MAKE) -C $(HOST_BRBOX_MKIMAGE_DIR) \
		Platform=local \
		CC="$(HOST_CC)" \
		STRIP="$(HOST_STRIP)" \
		EXTRA_CFLAGS="$(HOST_CFLAGS)" \
		EXTRA_CXXFLAGS="$(HOST_CXXFLAGS)" \
		EXTRA_LDFLAGS="$(HOST_LDFLAGS)" all
	touch $@

$(HOST_BRBOX_MKIMAGE_DIR)/.stamp_host_installed: $(HOST_BRBOX_MKIMAGE_DIR)/.stamp_built
	cp -a $(HOST_BRBOX_MKIMAGE_DIR)/brbox-mkimage $(HOST_DIR)/usr/bin/
	touch $@

host-brbox_mkimage: $(HOST_BRBOX_MKIMAGE_DIR)/.stamp_host_installed

host-brbox_mkimage-clean:
	-$(MAKE) -C $(HOST_BRBOX_MKIMAGE_DIR) Platform=local clean
	rm -f $(HOST_DIR)/usr/bin/brbox-mkimage
	rm -f .stamp_host_installed .stamp_built

host-brbox_mkimage-dirclean:
	rm -rf $(HOST_BRBOX_MKIMAGE_DIR)
#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(BR2_PACKAGE_HOST_BRBOX_MKIMAGE),y)
TARGETS+=host-brbox_mkimage
endif
