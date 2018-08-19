# Fiends And Fortune

_Fiends And Fortune_ is a personal playground where I turn old school paper and
pencil role playing rules into code.  Like the games that inspired this
project, these programs are old-school, command-line oriented and text based.
The code is modern C99.

## License

_Fiends And Fortune_ is available under a BSD-style license.  See the
[`LICENSE`][21] file for details.

[21]: https://github.com/donmccaughey/fiends_and_fortune/blob/master/LICENSE

## Building From Source

_Fiends And Fortune_ is developed on macOS and should work on most POSIX
compatible operating systems.  Building it requires that your system have
a C toolchain, [CMake][31] and the [`ncurses`][32] library installed.

    git clone https://github.com/donmccaughey/fiends_and_fortune.git
    cd fiends_and_fortune
    mkdir tmp
    cd tmp
    cmake -G "Unix Makefiles" ..
    make all test

[31]: https://cmake.org
[32]: https://invisible-island.net/ncurses/

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

