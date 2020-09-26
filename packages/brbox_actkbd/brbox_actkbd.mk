################################################################################
#
# actkbd daemon
#
################################################################################
BRBOX_ACTKBD_VERSION = master
BRBOX_ACTKBD_SITE = $(call github,hackboxguy,actkbd,$(BRBOX_ACTKBD_VERSION))
BRBOX_ACTKBD_INSTALL_STAGING = NO
BRBOX_ACTKBD_INSTALL_TARGET = YES
BRBOX_ACTKBD_LICENSE = GPL-2.0
BRBOX_ACTKBD_LICENSE_FILES = COPYING
BRBOX_ACTKBD_CONF_OPTS=-DAUTO_SVN_VERSION=OFF

$(eval $(cmake-package))
