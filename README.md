# Fiends And Fortune

_Fiends And Fortune_ is a personal playground where I turn old school paper and
pencil role playing rules into code.  Like the games that inspired this
project, these programs are old-school, command-line oriented and text based.
The code is modern C99.

[![GitHub Actions][11]][12] [![Code Coverage][13]][14]

[11]: https://github.com/donmccaughey/fiends_and_fortune/actions/workflows/tests.yml/badge.svg
[12]: https://github.com/donmccaughey/fiends_and_fortune/actions/workflows/tests.yml
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

_Fiends And Fortune_ is tested on macOS and Linux.  Building it requires that
your system have a C toolchain, a recent version of [CMake][31].  Building on
Linux requires [`libbsd`][32].

    git clone https://github.com/donmccaughey/fiends_and_fortune.git
    cd fiends_and_fortune
    cmake -S . -B tmp -DWALL=ON
    cmake --build tmp --target all test

This repository contains a copy of the [cJSON 1.7.15][33] source.

[31]: https://cmake.org
[32]: https://libbsd.freedesktop.org/
[33]: https://github.com/DaveGamble/cJSON


## Build System

_Fiends And Fortune_ uses [Cmake][41] as a build system, [GitHub Actions][42] 
for continuous integration, and [Codecov.io][43] for code coverage reporting.

### Build Options

To build with the [Address Sanitizer][44] enabled, set the `ADDRESS_SANITIZER`
option to `ON`.

    cmake -S . -B tmp -DADDRESS_SANITIZER=ON

Mac users note that the `clang` that ships with Xcode accepts the 
`-fsanitize=address` flag, but doesn't actually include the Address Sanitizer.

Set the `COVERAGE` option to `ON` to generate coverage files.

    cmake -S . -B tmp -DCOVERAGE=ON

Set the `WALL` option to `ON` turns on [additional warnings][44] using the 
`-Wall` compiler option and treats warnings as errors.  `WALL` is off by default
but should be turned on for development and integration builds.

    cmake -S . -B tmp -DCOVERAGE=ON -DWALL=ON

[41]: https://cmake.org
[42]: https://github.com/donmccaughey/fiends_and_fortune/actions/workflows/tests.yml
[43]: https://codecov.io/gh/donmccaughey/fiends_and_fortune
[44]: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options


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

