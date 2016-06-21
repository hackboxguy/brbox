#############################################################
#
# dispsrv (oled display control server)
#
#############################################################
BRBOX_DISPSRV_STARTNUM=$(call qstrip,$(BR2_PACKAGE_BRBOX_DISPSRV_INIT_ORDER))
BRBOX_DISPSRV_VERSION = 0.1
BRBOX_DISPSRV_SITE_METHOD = local
BRBOX_DISPSRV_SITE = $(TOPDIR)/../sources/services/dispsrv
BRBOX_DISPSRV_INSTALL_STAGING = YES
BRBOX_DISPSRV_INSTALL_TARGET = YES
BRBOX_DISPSRV_DEPENDENCIES = json-c brbox_libAdDisp brbox_libAdCmnOld brbox_libAdSttngs
BRBOX_DISPSRV_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF -DSTARTNUM=$(BRBOX_DISPSRV_STARTNUM)
$(eval $(cmake-package))

