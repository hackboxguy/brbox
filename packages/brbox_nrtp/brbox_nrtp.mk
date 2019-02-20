################################################################################
#
# brbox_nrtp
#
################################################################################
BRBOX_NRTP_VERSION = HEAD
BRBOX_NRTP_SITE =  https://github.com/mleeman/gst-plugins-rtp.git
BRBOX_NRTP_SITE_METHOD = git
BRBOX_NRTP_INSTALL_STAGING = YES
BRBOX_NRTP_DEPENDENCIES = gstreamer1 gst1-plugins-base
BRBOX_NRTP_LICENSE = LGPL-2.1
BRBOX_NRTP_LICENSE_FILES = COPYING

$(eval $(meson-package))

