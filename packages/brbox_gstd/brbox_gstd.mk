################################################################################
#
# brbox_gstd
#
################################################################################

BRBOX_GSTD_VERSION = v0.6.2
BRBOX_GSTD_SITE =  https://github.com/RidgeRun/gstd-1.x.git
BRBOX_GSTD_SITE_METHOD = git
BRBOX_GSTD_GIT_SUBMODULES = YES

BRBOX_GSTD_LICENSE = LGPL-2.1+
BRBOX_GSTD_LICENSE_FILES = COPYING

BRBOX_GSTD_AUTORECONF = YES
BRBOX_GSTD_DEPENDENCIES = host-pkgconf gstreamer1 gst1-plugins-base json-glib ncurses readline
#BRBOX_GSTD_CONF_OPTS = --disable-graphviz

$(eval $(autotools-package))
