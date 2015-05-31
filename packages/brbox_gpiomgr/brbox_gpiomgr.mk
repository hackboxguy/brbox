#############################################################
#
# gpiomgr (gpio control server)
#
#############################################################
BRBOX_GPIOMGR_STARTNUM=$(call qstrip,$(BR2_PACKAGE_BRBOX_GPIOMGR_INIT_ORDER))
BRBOX_GPIOMGR_VERSION = 0.1
BRBOX_GPIOMGR_SITE_METHOD = local
BRBOX_GPIOMGR_SITE = $(TOPDIR)/../sources/services/gpioctl
BRBOX_GPIOMGR_INSTALL_STAGING = YES
BRBOX_GPIOMGR_INSTALL_TARGET = YES
BRBOX_GPIOMGR_DEPENDENCIES = json-c brbox_libAdCmnOld brbox_libAdSttngs
BRBOX_GPIOMGR_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF -DSTARTNUM=$(BRBOX_GPIOMGR_STARTNUM)
$(eval $(cmake-package))

