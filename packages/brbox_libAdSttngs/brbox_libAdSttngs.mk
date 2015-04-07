################################################################################
#
# libAdSttngs
#
################################################################################
BRBOX_LIBADSTTNGS_DIR:=$(BUILD_DIR)/brbox_libAdSttngs

# Strip off the quoting
BRBOX_LIBADSTTNGS_SRC:=$(call qstrip,$(BR2_TARGET_BRBOX_SVNURL))/sources/lib/lib-settings

$(BRBOX_LIBADSTTNGS_DIR)/.stamp_downloaded:
	mkdir -p $(BUILD_DIR)
	svn co $(BRBOX_LIBADSTTNGS_SRC) $(BRBOX_LIBADSTTNGS_DIR)
	touch $@

$(BRBOX_LIBADSTTNGS_DIR)/.stamp_extracted: $(BRBOX_LIBADSTTNGS_DIR)/.stamp_downloaded
	touch $@

$(BRBOX_LIBADSTTNGS_DIR)/.stamp_configured: $(BRBOX_LIBADSTTNGS_DIR)/.stamp_extracted
	pushd .
	cd $(BRBOX_LIBADSTTNGS_DIR)	
	cmake $(BRBOX_LIBADSTTNGS_DIR) -DOUTBIN:STRING=$(TARGET_DIR)/usr/lib/
	popd
	touch $@

$(BRBOX_LIBADSTTNGS_DIR)/.stamp_built: $(BRBOX_LIBADSTTNGS_DIR)/.stamp_configured
	$(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(BRBOX_LIBADSTTNGS_DIR) \
		CROSS_COMPILE="$(TARGET_CROSS)" STRIP="$(TARGET_STRIP)" \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		EXTRA_CXXFLAGS="$(TARGET_CXXFLAGS)" \
		EXTRA_LDFLAGS="$(TARGET_LDFLAGS)" all
	touch $@

$(BRBOX_LIBADSTTNGS_DIR)/.stamp_target_installed: $(BRBOX_LIBADSTTNGS_DIR)/.stamp_built
	#mkdir -p $(TARGET_DIR)/usr/lib
	#cp -a $(BRBOX_LIBADSTTNGS_DIR)/build/libAdSttngs* $(TARGET_DIR)/usr/lib/
	touch $@

brbox_libAdSttngs: $(BRBOX_LIBADSTTNGS_DIR)/.stamp_target_installed

brbox_libAdSttngs-clean:
	-$(MAKE) -C $(BRBOX_LIBADSTTNGS_DIR)/build clean
	rm -f $(TARGET_DIR)/usr/lib/libAdSttngs*
	rm -f .stamp_target_installed .stamp_built

brbox_libAdSttngs-dirclean:
	rm -rf $(BRBOX_LIBADSTTNGS_DIR)
#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(BR2_PACKAGE_BRBOX_LIBADSTTNGS),y)
TARGETS+=brbox_libAdSttngs
endif
