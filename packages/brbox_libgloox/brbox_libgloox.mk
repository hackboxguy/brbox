################################################################################
#
# brbox_libgloox
#
################################################################################

BRBOX_LIBGLOOX_VERSION = 1.0.24 #4
BRBOX_LIBGLOOX_SOURCE = gloox-$(BRBOX_LIBGLOOX_VERSION).tar.bz2
BRBOX_LIBGLOOX_SITE = http://camaya.net/download
BRBOX_LIBGLOOX_INSTALL_STAGING = YES
BRBOX_LIBGLOOX_CONF_OPTS += --with-openssl=$(STAGING_DIR)/usr
BRBOX_LIBGLOOX_AUTORECONF = YES
BRBOX_LIBGLOOX_LICENSE = AGPLv3
BRBOX_LIBGLOOX_LICENSE_FILES = COPYING
BRBOX_LIBGLOOX_DEPENDENCIES += openssl
$(eval $(autotools-package))
