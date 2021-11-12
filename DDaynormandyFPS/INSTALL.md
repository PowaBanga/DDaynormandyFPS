DDayN Installation for Windows10
================================

Windows10 Install:
------------------

* Unzip the game where you want. 
And, if you have a windows security error message when you lauch the game:
   * Right click on DDaynormandy_x84.exe, select `Property`;
   * On the line `This file came from another computer may be blocked to help protect this computer`, check: `unblock` .


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

Get the sources
---------------

### Clone the repository, fetch submodules, enter the source tree:

```sh
git clone --recurse-submodules https://github.com/PowaBanga/DDaynormandyFPS.git
cd DDaynormandyFPS
```

If you already clone the repository and haven't fetched submodules you can fetch them this way (within _DDaynormandyFPS_ directory):

```sh
git submodule update --init --recursive
```

Build
-----

This is how to build the game in user directory:

```sh
make
```

You can optionaly use `-j2` to build using 2 cores or `-j$(nproc)`
to build using all available cores:

```sh
make -j$(nproc)
```

_Note:_ You can force target architecture for the compilation, this way:

```sh
make CPU=YOURARCH
```

Where YOURARCH is something like `x86` or `x86_64`.

_Note:_ If you compile this game on Windows, you have to place every `/dday/q2amdin*.txt` file in the main `/` folder.


Install
-------

To install the game system wide, you must build this way:

```sh
make CONFIG_PATH_DATA=/usr/local/share/games/ddaynormandy CONFIG_PATH_LIB=/usr/local/lib/games/ddaynormandy
```

And install this way:

```sh
sudo make install CONFIG_PATH_DATA=/usr/local/share/games/ddaynormandy CONFIG_PATH_LIB=/usr/local/lib/games/ddaynormandy
```
