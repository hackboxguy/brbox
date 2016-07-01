#############################################################
#
# eeprog i2c eeprom access utility
#
#############################################################
BRBOX_EEPROG_VERSION = 0.1
BRBOX_EEPROG_SITE_METHOD = local
BRBOX_EEPROG_SITE = $(TOPDIR)/../sources/utils/eeprog-0.7.6
BRBOX_EEPROG_INSTALL_STAGING = NO
BRBOX_EEPROG_INSTALL_TARGET = YES
BRBOX_EEPROG_CONF_OPTS=-DAUTO_SVN_VERSION=OFF #todo: when SITE_METHOD is svnco, enable AUTO_SVN_VERSION flag
#-DCMAKE_INSTALL_PREFIX="/opt/fmw/"
$(eval $(cmake-package))

