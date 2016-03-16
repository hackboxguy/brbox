################################################################################
#
# gstreamer1-vaapi
#
################################################################################

GSTREAMER_VAAPI_VERSION = 1.6.0
GSTREAMER_VAAPI_SITE = http://cgit.freedesktop.org/gstreamer/gstreamer-vaapi/snapshot #http://anongit.freedesktop.org/gstreamer/gstreamer-vaapi
GSTREAMER_SOURCE = gstreamer-vaapi-$(GSTREAMER_VAAPI_VERSION).tar.gz
GSTREAMER_VAAPI_SITE_METHOD = git_with_submodules
GSTREAMER_VAAPI_LICENSE = BSD-4c BSD-2c
GSTREAMER_VAAPI_INSTALL_STAGING = YES
GSTREAMER_VAAPI_DEPENDENCIES = host-pkgconf libva gstreamer1 gst1-plugins-bad gst1-plugins-ugly gst1-plugins-good libdrm  libglib2
GSTREAMER_VAAPI_CONF_OPTS = --enable-wayland=no --disable-builtin-libvpx
#GSTREAMER1_VAAPI_LIBTOOL_PATCH = NO



GSTREAMER_VAAPI_AUTORECONF      = YES

#GSTREAMER1_VAAPI_CONF_ENV = PKG_CONFIG_PATH=$(TARGET_DIR)/usr/lib/pkgconfig \
	YASM=$(HOST_DIR)/usr/bin/yasm \
	CFLAGS="-I$(TARGET_DIR)/usr/include -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -pipe -O1" \
	CPPFLAGS="-I$(TARGET_DIR)/usr/include -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -O1" \
	LDFLAGS="-L$(TARGET_DIR)/usr/lib -L$(STAGING_DIR)/usr/lib" \
	GST_CFLAGS="-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/usr/include/gstreamer-1.0 -pthread -I$(STAGING_DIR)/usr/include/glib-2.0 -I$(STAGING_DIR)/usr/lib/glib-2.0/include -I$(STAGING_DIR)/usr/include" \
	GST_BASE_LIBS="-L$(STAGING_DIR)/usr/lib -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -L$(STAGING_DIR)/usr/lib -lglib-2.0" \
	GST_LIBS="-L$(STAGING_DIR)/usr/lib -lgstreamer-1.0 -lgobject-2.0 -L$(STAGING_DIR)/usr/lib -lglib-2.0 "

define GSTREAMER_AUTOGEN_REPLACEMENT
        cd $(@D) && mkdir -p m4 && echo "EXTRA_DIST =" > gtk-doc.make
endef

GSTREAMER_VAAPI_PRE_CONFIGURE_HOOKS += GSTREAMER_AUTOGEN_REPLACEMENT

$(eval $(autotools-package))
