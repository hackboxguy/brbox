#############################################################
#
# modbussrv (modbus control server)
#
#############################################################
BRBOX_MODBUSSRV_STARTNUM=$(call qstrip,$(BR2_PACKAGE_BRBOX_MODBUSSRV_INIT_ORDER))
BRBOX_MODBUSSRV_VERSION = 0.1
BRBOX_MODBUSSRV_SITE_METHOD = local
BRBOX_MODBUSSRV_SITE = $(TOPDIR)/../sources/services/modbussrv
BRBOX_MODBUSSRV_INSTALL_STAGING = YES
BRBOX_MODBUSSRV_INSTALL_TARGET = YES
BRBOX_MODBUSSRV_DEPENDENCIES = json-c brbox_libAdCmnOld brbox_libAdSttngs
BRBOX_MODBUSSRV_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF -DSTARTNUM=$(BRBOX_MODBUSSRV_STARTNUM)
$(eval $(cmake-package))

