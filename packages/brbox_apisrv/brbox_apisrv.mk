#############################################################
#
# apisrv (raw-tcp-rpc to http-api gateway)
#
#############################################################
BRBOX_APISRV_STARTNUM=$(call qstrip,$(BR2_PACKAGE_BRBOX_APISRV_INIT_ORDER))
BRBOX_APISRV_VERSION = 0.1
BRBOX_APISRV_SITE_METHOD = local
BRBOX_APISRV_SITE = $(TOPDIR)/../sources/services/apisrv
BRBOX_APISRV_INSTALL_STAGING = YES
BRBOX_APISRV_INSTALL_TARGET = YES
BRBOX_APISRV_DEPENDENCIES = json-c brbox_libAdCmnOld brbox_libAdSttngs brbox_libonion
BRBOX_APISRV_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF -DSTARTNUM=$(BRBOX_APISRV_STARTNUM)
$(eval $(cmake-package))

