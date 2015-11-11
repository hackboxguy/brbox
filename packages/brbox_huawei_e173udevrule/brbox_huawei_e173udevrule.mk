#############################################################
#
# brbox huawei E173 udev rule(starts the 3g stick as gsm modem)
#
#############################################################
BRBOX_HUAWEI_E173UDEVRULE_SITE_METHOD = local
BRBOX_HUAWEI_E173UDEVRULE_SITE = $(TOPDIR)/../sources/scripts/cmn/huawei-E173-udev-rule
BRBOX_HUAWEI_E173UDEVRULE_INSTALL_STAGING = NO
BRBOX_HUAWEI_E173UDEVRULE_INSTALL_TARGET = YES
BRBOX_HUAWEI_E173UDEVRULE_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

