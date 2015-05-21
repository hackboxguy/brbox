################################################################################
#
# Intel i210 igb_avb module
#
################################################################################
#BARCO_I210KMODAVBIGB_SRC:=$(call qstrip,$("https://github.com/AVnu/Open-AVB/trunk/kmod/igb"))
#BARCO_I210KMODAVBIGB_SRC=http://github.com/AVnu/Open-AVB/trunk/kmod/igb
BARCO_I210KMODAVBIGB_VERSION = 0.1
BARCO_I210KMODAVBIGB_SITE_METHOD = local
BARCO_I210KMODAVBIGB_SITE = $(TOPDIR)/../sources/kmods/i210-igb-avb
BARCO_I210KMODAVBIGB_INSTALL_STAGING = NO
BARCO_I210KMODAVBIGB_INSTALL_TARGET = YES
BRBOX_I210KMODAVBIGB_LICENSE = GPLv2+
BARCO_I210KMODAVBIGB_DEPENDENCIES = linux
#There is no license file
#define ON2_8170_MODULES_BUILD_CMDS
#	$(MAKE) -C $(LINUX_DIR) $(LINUX_MAKE_FLAGS) M=$(@D)
#endef
#define ON2_8170_MODULES_INSTALL_TARGET_CMDS
#	$(MAKE) -C $(LINUX_DIR) $(LINUX_MAKE_FLAGS) M=$(@D) modules_install
#endef
$(eval $(generic-package))



