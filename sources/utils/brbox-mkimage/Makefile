#
# (C) Copyright 2002
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA

SOURCES := crc32.c  mkimage.c
HEADERS := image.h

ifeq ("$(Platform)","local")
	I386_FLAG=-m32
endif

all:	 version mkimage 

mkimage:	$(SOURCES) $(HEADERS)
	$(CROSS_COMPILE)gcc $(I386_FLAG) $(LFLAGS) -static -Wall -DUSE_HOSTCC $(SOURCES) -o brbox-mkimage

.PHONY: version
version:
	@echo -n "#define SRC_CONTROL_VERSION " > SrcControlVersion.h
	#svn info | grep "Revision:" | sed -e 's/.* //' >> SrcControlVersion.h
	git rev-list --count --first-parent HEAD >> SrcControlVersion.h
clean:
	rm -f SrcControlVersion.h
	rm -f brbox-mkimage

