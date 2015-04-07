################################################################################
#
# libAdSttngs
#
################################################################################
LIBADSTTNG_VERSION = 1.0
LIBADSTTNG_SOURCE = lib-settings.tar.gz
LIBADSTTNG_SITE = svn://github.com/hackboxguy/brbox/trunk/sources/released-tarballs
LIBADSTTNG_INSTALL_STAGING = YES
LIBADSTTNG_INSTALL_TARGET = YES
$(eval $(cmake-package))
