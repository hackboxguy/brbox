################################################################################
#
# gstreamer1-vaapi
#
################################################################################

BRBOX_GSTREAMER_VAAPI_VERSION = 1.8.2 #0.7.0 #1.6.0 (1.6.0 source has compile problem)
BRBOX_GSTREAMER_VAAPI_SITE = http://cgit.freedesktop.org/gstreamer/gstreamer-vaapi/snapshot
BRBOX_GSTREAMER_VAAPI_SOURCE = brbox_gstreamer_vaapi-$(BRBOX_GSTREAMER_VAAPI_VERSION).tar.gz
#BRBOX_GSTREAMER_VAAPI_SITE_METHOD = git_with_submodules
BRBOX_GSTREAMER_VAAPI_LICENSE = BSD-4c BSD-2c
BRBOX_GSTREAMER_VAAPI_INSTALL_STAGING = YES
BRBOX_GSTREAMER_VAAPI_DEPENDENCIES = host-pkgconf libva gstreamer1 gst1-plugins-bad gst1-plugins-ugly gst1-plugins-good libdrm  libglib2 xlib_libX11
BRBOX_GSTREAMER_VAAPI_CONF_OPTS = --enable-wayland=no --disable-builtin-libvpx
#BRBOX_GSTREAMER_VAAPI_LIBTOOL_PATCH = YES



BRBOX_GSTREAMER_VAAPI_AUTORECONF      = YES

#GSTREAMER1_VAAPI_CONF_ENV = PKG_CONFIG_PATH=$(TARGET_DIR)/usr/lib/pkgconfig \
#	YASM=$(HOST_DIR)/usr/bin/yasm \
#	CFLAGS="-I$(TARGET_DIR)/usr/include -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -pipe -O1" \
#	CPPFLAGS="-I$(TARGET_DIR)/usr/include -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -O1" \
#	LDFLAGS="-L$(TARGET_DIR)/usr/lib -L$(STAGING_DIR)/usr/lib" \
#	GST_CFLAGS="-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/usr/include/gstreamer-1.0 -pthread -I$(STAGING_DIR)/usr/include/glib-2.0 -I$(STAGING_DIR)/usr/lib/glib-2.0/include -I#$(STAGING_DIR)/usr/include" \
#	GST_BASE_LIBS="-L$(STAGING_DIR)/usr/lib -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -L$(STAGING_DIR)/usr/lib -lglib-2.0" \
#	GST_LIBS="-L$(STAGING_DIR)/usr/lib -lgstreamer-1.0 -lgobject-2.0 -L$(STAGING_DIR)/usr/lib -lglib-2.0 "

define BRBOX_GSTREAMER_AUTOGEN_REPLACEMENT
        cd $(@D) && mkdir -p m4 && echo "EXTRA_DIST =" > gtk-doc.make
        cd $(@D) && mkdir -p common/m4
endef

BRBOX_GSTREAMER_VAAPI_PRE_CONFIGURE_HOOKS += BRBOX_GSTREAMER_AUTOGEN_REPLACEMENT

$(eval $(autotools-package))
