#############################################################
#
# lightsensrv (light sensor service)
#
#############################################################
BRBOX_LIGHTSENSE_STARTNUM=$(call qstrip,$(BR2_PACKAGE_BRBOX_LIGHTSENSE_INIT_ORDER))
BRBOX_LIGHTSENSE_VERSION = 0.1
BRBOX_LIGHTSENSE_SITE_METHOD = local
BRBOX_LIGHTSENSE_SITE = $(TOPDIR)/../sources/services/lightsense
BRBOX_LIGHTSENSE_INSTALL_STAGING = YES
BRBOX_LIGHTSENSE_INSTALL_TARGET = YES
BRBOX_LIGHTSENSE_DEPENDENCIES = i2c-tools json-c brbox_libAdCmnOld brbox_libAdSttngs
BRBOX_LIGHTSENSE_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF -DSTARTNUM=$(BRBOX_LIGHTSENSE_STARTNUM)
$(eval $(cmake-package))

