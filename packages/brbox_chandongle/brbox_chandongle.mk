#############################################################
#
## brbox-chan-dongle
#
##############################################################
BRBOX_CHANDONGLE_VERSION = master
BRBOX_CHANDONGLE_SOURCE = $(BRBOX_CHANDONGLE_VERSION).tar.gz
BRBOX_CHANDONGLE_SITE = https://github.com/wdoekes/asterisk-chan-dongle/archive
BRBOX_CHANDONGLE_INSTALL_TARGET = YES
BRBOX_CHANDONGLE_CONF_OPTS = --with-astversion=16.14 --with-asterisk="$(STAGING_DIR)/usr/include" --with-sqlite3=no 
#--with-sqlite3="$(STAGING_DIR)/usr/include"
BRBOX_CHANDONGLE_CONF_ENV += CPPFLAGS="-I$(STAGING_DIR)/usr/include/"
BRBOX_CHANDONGLE_DEPENDENCIES = sqlite asterisk
define BRBOX_CHANDONGLE_PRE_CONFIGURE_BOOTSTRAP
    cd $(@D)/ && ./bootstrap
endef

BRBOX_CHANDONGLE_PRE_CONFIGURE_HOOKS += BRBOX_CHANDONGLE_PRE_CONFIGURE_BOOTSTRAP

$(eval $(autotools-package))
