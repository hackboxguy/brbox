################################################################################
#
# Intel i210 igb_avb module
#
###############################################################################

BRBOX_I210KMODAVBIGB_SRC="http://github.com/AVnu/Open-AVB/trunk/kmod/igb"
BRBOX_I210KMODAVBIGB_DIR:=$(BUILD_DIR)/brbox_I210kmodAvbIgb
# Strip off the quoting
#BRBOX_I210KMODAVBIGB_SRC:=$(call qstrip,$(BRBOX_I210KMODAVBIGB_SRCPATH))

$(BRBOX_I210KMODAVBIGB_DIR)/.stamp_downloaded:
	mkdir -p $(BUILD_DIR)
	svn co $(BRBOX_I210KMODAVBIGB_SRC) $(BRBOX_I210KMODAVBIGB_DIR)
	touch $@

$(BRBOX_I210KMODAVBIGB_DIR)/.stamp_extracted: $(BRBOX_I210KMODAVBIGB_DIR)/.stamp_downloaded
	touch $@

$(BRBOX_I210KMODAVBIGB_DIR)/.stamp_configured: $(BRBOX_I210KMODAVBIGB_DIR)/.stamp_extracted
	touch $@

$(BRBOX_I210KMODAVBIGB_DIR)/.stamp_built: linux $(BRBOX_I210KMODAVBIGB_DIR)/.stamp_configured
	$(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(BRBOX_I210KMODAVBIGB_DIR) \
		ARCH=$(KERNEL_ARCH) \
		CC="$(TARGET_CC)" STRIP="$(TARGET_STRIP)" \
		CROSS_COMPILE="$(CCACHE) $(TARGET_CROSS)" \
		EXTRA_CFLAGS="-DCONFIG_PTP" \
		EXTRA_CXXFLAGS="$(TARGET_CXXFLAGS)" \
		EXTRA_LDFLAGS="$(TARGET_LDFLAGS)" \
		KSRC="$(LINUX_DIR)" default
	touch $@

$(BRBOX_I210KMODAVBIGB_DIR)/.stamp_target_installed: $(BRBOX_I210KMODAVBIGB_DIR)/.stamp_built
	mkdir -p $(BRBOX_I210KMODAVBIGB_DIR)/opt/fmw/lib/modules
	cp -a $(BRBOX_I210KMODAVBIGB_DIR)/igb_avb.ko $(TARGET_DIR)/opt/fmw/lib/modules
	touch $@

brbox_I210kmodAvbIgb: $(BRBOX_I210KMODAVBIGB_DIR)/.stamp_target_installed

brbox_I210kmodAvbIgb-clean:
	-$(MAKE) -C $(BRBOX_I210KMODAVBIGB_DIR) PLATFORM=arm ARCH=arm clean
	rm -f $(TARGET_DIR)/opt/fmw/lib/modules/igb_avb.ko

brbox_I210kmodAvbIgb-dirclean:
	rm -rf $(BRBOX_I210KMODAVBIGB_DIR)

ifeq ($(BR2_PACKAGE_BRBOX_I210KMODAVBIGB),y)
PACKAGES+=brbox_I210kmodAvbIgb
endif
