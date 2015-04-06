#############################################################
#
# modified mkimage for BRBOX use
#
#############################################################

BRBOX_MKIMAGE_DIR:=$(BUILD_DIR)/brbox_mkimage
BRBOX_MKIMAGE_BIN:=$(BRBOX_MKIMAGE_DIR)/build

# Strip off the annoying quoting
#BRBOX_MKIMAGE_SRC:=$(call qstrip,$(BR2_PACKAGE_BRBOX_MKIMAGE_SOURCE))
BRBOX_MKIMAGE_SRC:=$(call qstrip,$(BR2_TARGET_BRBOX_SVNURL))/sources/utils/brbox-mkimage

$(BRBOX_MKIMAGE_DIR)/.stamp_downloaded:
	mkdir -p $(BUILD_DIR)
	svn co $(BRBOX_MKIMAGE_SRC) $(BRBOX_MKIMAGE_DIR)
	touch $@

$(BRBOX_MKIMAGE_DIR)/.stamp_extracted: $(BRBOX_MKIMAGE_DIR)/.stamp_downloaded
	touch $@

$(BRBOX_MKIMAGE_DIR)/.stamp_configured: $(BRBOX_MKIMAGE_DIR)/.stamp_extracted
	touch $@

$(BRBOX_MKIMAGE_DIR)/.stamp_built: $(BRBOX_MKIMAGE_DIR)/.stamp_configured
	$(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(BRBOX_MKIMAGE_DIR) \
		CROSS_COMPILE="$(TARGET_CROSS)" STRIP="$(TARGET_STRIP)" \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		EXTRA_CXXFLAGS="$(TARGET_CXXFLAGS)" \
		EXTRA_LDFLAGS="$(TARGET_LDFLAGS)" all
	touch $@

$(BRBOX_MKIMAGE_DIR)/.stamp_target_installed: $(BRBOX_MKIMAGE_DIR)/.stamp_built
	mkdir -p $(TARGET_DIR)/usr/sbin
	cp -a $(BRBOX_MKIMAGE_DIR)/brbox-mkimage $(TARGET_DIR)/usr/sbin/
	touch $@

brbox_mkimage: $(BRBOX_MKIMAGE_DIR)/.stamp_target_installed

brbox_mkimage-clean:
	-$(MAKE) -C $(BRBOX_MKIMAGE_DIR) clean
	rm -f $(TARGET_DIR)/usr/sbin/brbox-mkimage
	rm -f .stamp_target_installed .stamp_built

brbox_mkimage-dirclean:
	rm -rf $(BRBOX_MKIMAGE_DIR)
#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(BR2_PACKAGE_BRBOX_MKIMAGE),y)
TARGETS+=brbox_mkimage
endif
