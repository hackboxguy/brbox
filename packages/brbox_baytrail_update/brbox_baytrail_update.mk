#############################################################
#
# brbox baytrail update scripts
#
#############################################################

BRBOX_BAYTRAIL_UPDATE_DIR:=$(BUILD_DIR)/brbox_baytrail_update

# Strip off the annoying quoting
#BRBOX_BAYTRAIL_UPDATE_SRC:=$(call qstrip,$(BR2_PACKAGE_BRBOX_BAYTRAIL_UPDATE_SOURCE))
BRBOX_BAYTRAIL_UPDATE_SRC:=$(call qstrip,$(BR2_TARGET_BRBOX_SVNURL))/sources/scripts/baytrail_update

$(BRBOX_BAYTRAIL_UPDATE_DIR)/.stamp_downloaded:
	mkdir -p $(BUILD_DIR)
	svn co $(BRBOX_BAYTRAIL_UPDATE_SRC) $(BRBOX_BAYTRAIL_UPDATE_DIR)
	touch $@

$(BRBOX_BAYTRAIL_UPDATE_DIR)/.stamp_extracted: $(BRBOX_BAYTRAIL_UPDATE_DIR)/.stamp_downloaded
	touch $@

$(BRBOX_BAYTRAIL_UPDATE_DIR)/.stamp_configured: $(BRBOX_BAYTRAIL_UPDATE_DIR)/.stamp_extracted
	touch $@

$(BRBOX_BAYTRAIL_UPDATE_DIR)/.stamp_built: $(BRBOX_BAYTRAIL_UPDATE_DIR)/.stamp_configured
	touch $@

$(BRBOX_BAYTRAIL_UPDATE_DIR)/.stamp_target_installed: $(BRBOX_BAYTRAIL_UPDATE_DIR)/.stamp_built
	mkdir -p $(TARGET_DIR)/usr/sbin
	cp -a $(BRBOX_BAYTRAIL_UPDATE_DIR)/baytrail-brdskmgr $(TARGET_DIR)/usr/sbin/brdskmgr
	cp -a $(BRBOX_BAYTRAIL_UPDATE_DIR)/baytrail-update   $(TARGET_DIR)/usr/sbin/update
	touch $@

brbox_baytrail_update: $(BRBOX_BAYTRAIL_UPDATE_DIR)/.stamp_target_installed

brbox_baytrail_update-clean:
	rm -f $(TARGET_DIR)/usr/bin/update
	rm -f $(TARGET_DIR)/usr/bin/brdskmgr
	rm -f .stamp_*_installed

brbox_baytrail_update-dirclean:
	rm -rf $(BRBOX_BAYTRAIL_UPDATE_DIR)
#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(BR2_PACKAGE_BRBOX_BAYTRAIL_UPDATE),y)
TARGETS+=brbox_baytrail_update
endif
