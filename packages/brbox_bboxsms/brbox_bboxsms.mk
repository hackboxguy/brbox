#############################################################
#
# bboxsms (3g modem sms control server)
#
#############################################################
BRBOX_BBOXSMS_STARTNUM=$(call qstrip,$(BR2_PACKAGE_BRBOX_BBOXSMS_INIT_ORDER))
BRBOX_BBOXSMS_VERSION = 0.1
BRBOX_BBOXSMS_SITE_METHOD = local
BRBOX_BBOXSMS_SITE = $(TOPDIR)/../sources/services/bboxsms
BRBOX_BBOXSMS_INSTALL_STAGING = YES
BRBOX_BBOXSMS_INSTALL_TARGET = YES
BRBOX_BBOXSMS_DEPENDENCIES = json-c brbox_libAdCmnOld brbox_libAdSttngs brbox_libgammu
BRBOX_BBOXSMS_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF -DSTARTNUM=$(BRBOX_BBOXSMS_STARTNUM)
$(eval $(cmake-package))

