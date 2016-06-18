#############################################################
#
# i2csrv (i2c control server)
#
#############################################################
BRBOX_I2CSRV_STARTNUM=$(call qstrip,$(BR2_PACKAGE_BRBOX_I2CSRV_INIT_ORDER))
BRBOX_I2CSRV_VERSION = 0.1
BRBOX_I2CSRV_SITE_METHOD = local
BRBOX_I2CSRV_SITE = $(TOPDIR)/../sources/services/i2csrv
BRBOX_I2CSRV_INSTALL_STAGING = YES
BRBOX_I2CSRV_INSTALL_TARGET = YES
BRBOX_I2CSRV_DEPENDENCIES = json-c brbox_libAdCmnOld brbox_libAdSttngs
BRBOX_I2CSRV_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/opt/fmw/" -DAUTO_SVN_VERSION=OFF -DINSTALL_MY_HEADERS=OFF -DSTARTNUM=$(BRBOX_I2CSRV_STARTNUM)
$(eval $(cmake-package))

