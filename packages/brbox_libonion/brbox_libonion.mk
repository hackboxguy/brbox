################################################################################
#
# brbox_libonion
#
################################################################################
BRBOX_LIBONION_VERSION = v0.7
BRBOX_LIBONION_SITE = $(call github,davidmoreno,onion,$(BRBOX_LIBONION_VERSION))
BRBOX_LIBONION_INSTALL_STAGING = YES
BRBOX_LIBONION_CONF_OPTS = -DONION_USE_SSL=false -DONION_USE_PAM=false -DONION_USE_SYSTEMD=false -DONION_USE_SQLITE3=false -DONION_USE_GC=false -DONION_USE_TESTS=false -DONION_EXAMPLES=false -DONION_USE_BINDINGS_CPP=false -DONION_USE_JPEG=false -DONION_USE_PNG=false
BRBOX_LIBONION_LICENSE = GPLv2.0
BRBOX_LIBONION_LICENSE_FILES = COPYING.lib

$(eval $(cmake-package))

