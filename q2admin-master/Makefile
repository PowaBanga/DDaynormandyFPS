GIT_REV=$(shell git rev-parse --short HEAD)
ifeq ($(GIT_REV),)
GIT_REV=git
endif

REL_VERSION=2.0
VERSION=$(REL_VERSION)~$(GIT_REV)

# remember to update this for every release commit
RELEASE=no

ifeq ($(RELEASE),yes)
VERSION=$(REL_VERSION)
endif

LUA_CFLAGS = $(shell pkg-config --cflags lua5.1)
LUA_LDFLAGS = $(shell pkg-config --libs lua5.1)

LDFLAGS = -lm $(LUA_LDFLAGS)

CC=gcc

CFLAGS ?= -ffast-math -O3
ifdef DEBUG
 CFLAGS = -O -g
endif
CFLAGS+=-Wall -Wextra -Werror -Wfatal-errors -fPIC $(LUA_CFLAGS)

PLATFORM=$(shell uname -s|tr A-Z a-z)

# only tested on linux currently
ifneq ($(PLATFORM),linux)
 #ifneq ($(PLATFORM),freebsd)
  #ifneq ($(PLATFORM),darwin)
   $(error OS $(PLATFORM) is currently not supported)
  #endif
 #endif
endif

ARCH=$(shell uname -m | sed -e s/i.86/i386/ -e s/sun4u/sparc/ -e s/sparc64/sparc/ -e s/arm.*/arm/ -e s/sa110/arm/ -e s/alpha/axp/)

SHLIBEXT=so
GAME_NAME=game$(ARCH).$(SHLIBEXT)

MAKE_FLAGS = CC=$(CC) CFLAGS="$(CFLAGS) -DQ2A_VERSION=\\\"$(VERSION)\\\"" LDFLAGS="$(LDFLAGS)" GAME_NAME=$(GAME_NAME)

all:
	cd src && make -j2 all $(MAKE_FLAGS)

clean:
	cd src && make clean $(MAKE_FLAGS)
	rm -f $(GAME_NAME)
