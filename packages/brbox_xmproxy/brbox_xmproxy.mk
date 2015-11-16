#############################################################
#
# xmproxy (xmpp client agent)
#
#############################################################
BRBOX_XMPROXY_STARTNUM=$(call qstrip,$(BR2_PACKAGE_BRBOX_XMPROXY_INIT_ORDER))
BRBOX_XMPROXY_VERSION = 0.1
BRBOX_XMPROXY_SITE_METHOD = local
BRBOX_XMPROXY_SITE = $(TOPDIR)/../sources/services/xmproxy
BRBOX_XMPROXY_INSTALL_STAGING = YES
BRBOX_XMPROXY_INSTALL_TARGET = YES
BRBOX_XMPROXY_DEPENDENCIES = json-c brbox_libAdCmnOld brbox_libAdSttngs brbox_libgloox
BRBOX_XMPROXY_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF -DSTARTNUM=$(BRBOX_XMPROXY_STARTNUM)
$(eval $(cmake-package))

