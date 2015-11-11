################################################################################
#
# brbox_libgammu
#
################################################################################
BRBOX_LIBGAMMU_VERSION = 1.36.6
BRBOX_LIBGAMMU_SITE = $(call github,gammu,gammu,$(BRBOX_LIBGAMMU_VERSION))
BRBOX_LIBGAMMU_INSTALL_STAGING = YES
BRBOX_LIBGAMMU_CONF_OPTS = -DWITH_NOKIA_SUPPORT=OFF -DWITH_BLUETOOTH=OFF -DWITH_IRDA=OFF
BRBOX_LIBGAMMU_LICENSE = GPLv2.0
BRBOX_LIBGAMMU_LICENSE_FILES = COPYING.lib

$(eval $(cmake-package))

