#############################################################
#
# mplaysrv (media-player server)
#
#############################################################
BRBOX_MPLAYSRV_STARTNUM=$(call qstrip,$(BR2_PACKAGE_BRBOX_MPLAYSRV_INIT_ORDER))
BRBOX_MPLAYSRV_VERSION = 0.1
BRBOX_MPLAYSRV_SITE_METHOD = local
BRBOX_MPLAYSRV_SITE = $(TOPDIR)/../sources/services/hotwire #hotwire has been renamed to mplaysrv
BRBOX_MPLAYSRV_INSTALL_STAGING = YES
BRBOX_MPLAYSRV_INSTALL_TARGET = YES
BRBOX_MPLAYSRV_DEPENDENCIES = json-c brbox_libAdCmnOld brbox_libAdSttngs opencv
BRBOX_MPLAYSRV_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF -DSTARTNUM=$(BRBOX_MPLAYSRV_STARTNUM)
$(eval $(cmake-package))

