DDayN compilation
=================

Dependencies
------------

```
libc6-dev libx11-dev libsdl1.2-dev libopenal-dev
libpng12-dev libjpeg8-dev zlib1g-dev mesa-common-dev
liblircclient-dev libcurl4-gnutls-dev
```

You can use `libjpeg62-dev` instead of `libjpeg8-dev` or
`libcurl4-openssl-dev` instead of `libcurl4-gnutls-dev` as you
like.

Build/Install
-------------

This is how to build the game in user directory:

```
make -C q2proSRC
cp q2proSRC/q2pro .
make -C DDaySRC
cp DDaySRC/game*.* dday/
```

To install the game system wide, first of all you must edit
the `q2proSRC/.config` file this way:

```
CONFIG_PATH_DATA=/usr/local/share/games/ddaynormandy
CONFIG_PATH_LIB=/usr/local/lib/games/ddaynormandy
```

then paste `ddaySRC/game*.so` and `dday/` in the right directories.

See `q2proSRC/INSTALL` for more information.
