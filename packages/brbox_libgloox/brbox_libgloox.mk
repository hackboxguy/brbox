################################################################################
#
# brbox_libgloox
#
################################################################################

BRBOX_LIBGLOOX_VERSION = 1.0.9
BRBOX_LIBGLOOX_SOURCE = gloox-$(BRBOX_LIBGLOOX_VERSION).tar.bz2
BRBOX_LIBGLOOX_SITE = http://camaya.net/download
BRBOX_LIBGLOOX_INSTALL_STAGING = YES
BRBOX_LIBGLOOX_AUTORECONF = YES
BRBOX_LIBGLOOX_LICENSE = AGPLv3
BRBOX_LIBGLOOX_LICENSE_FILES = COPYING

$(eval $(autotools-package))
