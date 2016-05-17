################################################################################
#
# generic-interrupt-driver
#
################################################################################
#BRBOX_GENINTRDRV_STARTSCR=$(call qstrip,$(BR2_PACKAGE_BRBOX_GENINTRDRV_INIT_SCRIPT))
BRBOX_GENINTRDRV_VERSION = 0.1
BRBOX_GENINTRDRV_SITE_METHOD = local
BRBOX_GENINTRDRV_SITE = $(TOPDIR)/../sources/kmods/genintrdrv
BRBOX_GENINTRDRV_INSTALL_TARGET = YES
BRBOX_GENINTRDRV_LICENSE = GPLv2
BRBOX_GENINTRDRV_LICENSE_FILES = COPYING
BRBOX_GENINTRDRV_NAME = genintrdrv
BRBOX_GENINTRDRV_DEPENDENCIES = linux

#define BRBOX_GENINTRDRV_BUILD_CMDS
#    #make sure that obj-y += STATICDRVR/ is only in the build makefile once
#    sed -i '/obj-y += STATICDRVR/d' $(BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/Makefile
#    echo "obj-y += STATICDRVR/" >> $(BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/Makefile
#    rm -rf $(BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/STATICDRVR
#    cp -r $(@D)/src $(BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/STATICDRVR
#    echo "obj-y += driver.o" > $(BUILD_DIR)/linux-$(LINUX_VERSION)/drivers/STATICDRVR/Makefile
#endef
#define BRBOX_GENINTRDRV_INSTALL_STAGING_CMDS
#endef
define BRBOX_GENINTRDRV_INSTALL_INIT_SYSV
        $(INSTALL) -D -m 0755 package/brbox/brbox_genintrdrv/S59GenIntrDrvModule \
                $(TARGET_DIR)/etc/init.d/S59GenIntrDrvModule
endef

#define BRBOX_GENINTRDRV_DEVICES
#endef
#define BRBOX_GENINTRDRV_PERMISSIONS
#endef
#define BRBOX_GENINTRDRV_USERS
#endef
$(eval $(kernel-module))
$(eval $(generic-package))

