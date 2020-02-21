################################################################################
#
# brbox_fbv_improved
#
################################################################################
BRBOX_FIM_VERSION = 0.6
BRBOX_FIM_SOURCE = fim-$(BRBOX_FIM_VERSION)-trunk.tar.gz
BRBOX_FIM_SITE = http://download.savannah.nongnu.org/releases/fbi-improved
BRBOX_FIM_LICENSE = GPL-2.0+
BRBOX_FIM_LICENSE_FILES = LICENSE
BRBOX_FIM_DEPENDENCIES += host-pkgconf libexif libpng ncurses readline flex
#BRBOX_FIM_CONF_ENV += libpng_config_prefix=$(STAGING_DIR)/usr/bin/libpng-config
#BRBOX_FIM_CONF_ENV += fim_cv_regex=no
BRBOX_FIM_CONF_ENV += ac_cv_path_LIBPNG_CONFIG=$(STAGING_DIR)/usr/bin/libpng-config
BRBOX_FIM_AUTORECONF = YES
BRBOX_FIM_CONF_OPTS += --enable-libpng --disable-exif 
$(eval $(autotools-package))
