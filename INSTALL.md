DDayN Installation for Windows10:
=================================
Windows10 Install:
------------------
 * Unzip the game where do you want. 
   And, if you have a windows secutity error message when you lauch the game:
   * Right clic on DDaynormandy_x84.exe -> Property
   * On the line `This file came from another computer may be blocked to help protect this computer` check: `unblock` 
 
 
DDayN compilation
=================
Dependencies
------------

```
libc6-dev libx11-dev libSDL2-dev libsdl1.2-dev libopenal-dev
libpng12-dev libjpeg8-dev zlib1g-dev mesa-common-dev
liblircclient-dev libcurl4-gnutls-dev
```

You can use `libjpeg62-dev` instead of `libjpeg8-dev` or
`libcurl4-openssl-dev` instead of `libcurl4-gnutls-dev` as you
like.

Build
-----

This is how to build the game in user directory:

`make`

You can optionaly use `-j2` to build using 2 cores or `-j$(nproc)`
to build using all available cores:

`make -j$(nproc)`

_note :_

If you want to force arch of the compilation, you have to modify this line in makefile with a text editor :

`echo "ARCH=$(arch)" -> variable.mk` by `echo "ARCH=YOURARCH" -> variable.mk`


and modify too in this file  `/q2proSRC/.config`:

`#CPU=x86` by `CPU=YOURARCH`

_Note_ about windows
If you compil this game on windows, you have to place all `/dday/q2amdin*.txt` in the main folder `/`


Install
-------

To install the game system wide, first of all you must edit
the `q2proSRC/.config` file this way:

```
CONFIG_PATH_DATA=/usr/local/share/games/ddaynormandy
CONFIG_PATH_LIB=/usr/local/lib/games/ddaynormandy
```

then build using `make` as explained above,

then paste `ddaySRC/game*.so` and `dday/` in the right directories.

See `q2proSRC/INSTALL` for more information.
