#############################################################
#
# smarteye (smarteye control server)
#
#############################################################
BRBOX_SMARTEYE_VERSION = 0.1
BRBOX_SMARTEYE_SITE_METHOD = local
BRBOX_SMARTEYE_SITE = $(TOPDIR)/../sources/services/smarteye
BRBOX_SMARTEYE_INSTALL_STAGING = YES
BRBOX_SMARTEYE_INSTALL_TARGET = YES
BRBOX_SMARTEYE_DEPENDENCIES = json-c brbox_libAdCmnOld brbox_libAdSttngs opencv
BRBOX_SMARTEYE_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF
$(eval $(cmake-package))

