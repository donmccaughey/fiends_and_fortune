# Fiends And Fortune

_Fiends And Fortune_ is a personal playground where I turn old school paper and
pencil role playing rules into code.  Like the games that inspired this
project, these programs are old-school, command-line oriented and text based.
The code is modern C99.

[![Build Status][11]][12] [![Code Coverage][13]][14]

[11]: https://travis-ci.org/donmccaughey/fiends_and_fortune.svg?branch=master
[12]: https://travis-ci.org/donmccaughey/fiends_and_fortune
[13]: https://codecov.io/gh/donmccaughey/fiends_and_fortune/branch/master/graph/badge.svg
[14]: https://codecov.io/gh/donmccaughey/fiends_and_fortune


## License

_Fiends And Fortune_ is available under a BSD-style license.  See the
[`LICENSE`][21] file for details.  The [_cJSON_ library][22] is available under 
an [MIT license][23].

[21]: https://github.com/donmccaughey/fiends_and_fortune/blob/master/LICENSE
[22]: https://github.com/donmccaughey/fiends_and_fortune/blob/master/libs/cJSON
[23]: https://github.com/donmccaughey/fiends_and_fortune/blob/master/libs/cJSON/LICENSE


## Building From Source

_Fiends And Fortune_ is developed on macOS and should work on most POSIX
compatible operating systems.  Building it requires that your system have
a C toolchain, a recent version of [CMake][31] and the [`ncurses`][32] library 
installed.

    git clone https://github.com/donmccaughey/fiends_and_fortune.git
    cd fiends_and_fortune
    cmake -S . -B tmp
    cmake --build tmp --target all test

Note that on MacOS 10.14 Mojave with Xcode 10.3, linking to `ncurses` `libmenu` 
fails with "Undefined symbols for architecture x86_64" errors.  You can 
[work around this][33] by installing `ncurses` using [Homebrew][34].

    brew install ncurses

This repository contains a copy of the [cJSON 1.7.7][35] source.

[31]: https://cmake.org
[32]: https://invisible-island.net/ncurses/
[33]: https://stackoverflow.com/questions/56622042/clang-on-macos-fails-linking-lmenu-from-ncurses
[34]: https://brew.sh
[35]: https://github.com/DaveGamble/cJSON


## Build System

_Fiends And Fortune_ uses [Cmake][41] as a build system, [Travis CI][42] for
continuous integration, and [Codecov.io][43] for code coverage reporting.

Currently, the [Codecov bash script][44] is pinned at [commit 8b76995][45] from 
8-Feb-2018, since the following commit [breaks uploading for Cmake projects][46].

[41]: https://cmake.org
[42]: https://travis-ci.org/donmccaughey/fiends_and_fortune
[43]: https://codecov.io/gh/donmccaughey/fiends_and_fortune
[44]: https://github.com/codecov/codecov-bash
[45]: https://github.com/codecov/codecov-bash/commit/8b76995ad4a95a61cecd4b049a448a402d91d197
[46]: https://github.com/codecov/codecov-bash/issues/162


## Motivation

I was an avid role player and game master in high school and college, spending
hours with friends enjoying Dungeons & Dragons, Tunnels & Trolls, Traveller,
RuneQuest and many others.  Aspects of these games screamed out to be
automated, which I tried to do for various games at various times in numerous
languages, including TRS-80 BASIC, Pascal, dBase, Excel macros, C++, Visual
BASIC, Java, Python, Scala, Objective-C -- you get the picture.  I'm sure
there's a half-finished project in a half-learned language I'm forgetting here.

The progression of half-baked attempts to encode various rule systems closely
tracks my development as a programmer.  I find this type of project to be a
great way to really learn a new language or framework; the complexity and
ambiguity inherent in game rules designed for humans is a good example of the
types of real world problems you have to tackle as a programmer.  Unfortunately
most of these RPG projects were abandoned part-way through.  They provided a
good learning experience, then inevitably my interests moved on.

Though I still enjoy learning new languages, the C language has become and
remained a favorite over the years.  In 2012 I decided to automate some of the
old treasure generation tables from one of the old fantasy games I spent
countless hours playing.  I've returned to this project periodically since
then.

