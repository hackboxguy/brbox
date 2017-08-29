#############################################################
#
# brbox sample-media files(ex: mkv/mp4 audio/video files)
#
#############################################################
BRBOX_SAMPLEMEDIA_SITE_METHOD = local
BRBOX_SAMPLEMEDIA_SITE = $(TOPDIR)/../sources/sample-media
BRBOX_SAMPLEMEDIA_INSTALL_STAGING = NO
BRBOX_SAMPLEMEDIA_INSTALL_TARGET = YES
BRBOX_SAMPLEMEDIA_CONF_OPTS=-DCMAKE_INSTALL_PREFIX="/"
$(eval $(cmake-package))

