#############################################################
#
# sysmgr server
#
#############################################################
BRBOX_SYSMGR_VERSION = 0.1
BRBOX_SYSMGR_SITE_METHOD = local
BRBOX_SYSMGR_SITE = $(TOPDIR)/../sources/services/sysmgr
BRBOX_SYSMGR_INSTALL_STAGING = YES
BRBOX_SYSMGR_INSTALL_TARGET = YES
BRBOX_SYSMGR_DEPENDENCIES = json-c brbox_libAdCmnOld brbox_libAdSttngs
BRBOX_SYSMGR_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF
$(eval $(cmake-package))

