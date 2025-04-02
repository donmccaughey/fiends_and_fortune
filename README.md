

_Fiends And Fortune_ is a translation of paper and pencil role playing rules
into code.  Like the games that inspired this project, these programs are 
old-school, command-line oriented and text based.  The core code is C99; I 
grudgingly use C++14 for the [TUI][1].

[1]: https://en.wikipedia.org/wiki/Text-based_user_interface


[![GitHub Actions][11]][12] [![Code Coverage][13]][14]

[11]: https://github.com/donmccaughey/fiends_and_fortune/actions/workflows/tests.yml/badge.svg?branch=main
[12]: https://github.com/donmccaughey/fiends_and_fortune/actions/workflows/tests.yml
[13]: https://codecov.io/gh/donmccaughey/fiends_and_fortune/branch/main/graph/badge.svg
[14]: https://codecov.io/gh/donmccaughey/fiends_and_fortune


## Modules

_Fiends And Fortune_ has modules for generating characters, dungeons, magic 
items and treasure.  Currently, the character generator is very nascent; the
dungeon generator is the most mature but still has some missing features and
odd behavior.


## Executables

_Fiends And Fortune_ contains two executables: `fnf` and `fiends`.  The `fnf`
executable is a command-line tool that generates output as human-readable text
and JSON.  `fiends` is a full-screen text UI with a familiar menu and windows
interface.


## `fnf` Options

    fnf: no action given
    Usage: fnf [OPTIONS] ACTION
    
      -d, --debug         print debugging information
      -h, --help          display this help message and exit
      -j, --jrand48=SEED  use the jrand48 random number generator
                            with the given 48-bit SEED
      ---format=FORMAT    output format where FORMAT is
                            `text' or `json' (default `text'
      -v, --verbose       print more details
    
    Available actions:
      character [METHOD]  generate a character where METHOD is
                            `method1', `method2', `method3', `method4',
                            `generalnpc', `specialnpc' or `simple'
                            (default `simple')
      check [N]           run tests where N is the "constant"
                            random number (default 0)
      dungeon [TYPE]      generate a dungeon where TYPE is
                            `random' or `small' (default `random')
      each                generate one of each treasure
      magic [COUNT]       generate COUNT magic items (default 10)
      map                 generate one treasure map
      table               generate the treasure type table
      LETTER              generate the treasure type for LETTER (A-Z)


## `fiends` Screen Shot

![`fiends` Screen Shot][21]

[21]: https://github.com/donmccaughey/fiends_and_fortune/blob/main/docs/images/fiends-screenshot-1.png


## Building From Source

_Fiends And Fortune_ is tested on macOS and Linux.  Building it requires that
your system have a C/C++ toolchain and [CMake][31] 3.13 or later.  Building on
Linux requires [`libbsd`][32] and [`libgpm`][33].

    git clone https://github.com/donmccaughey/fiends_and_fortune.git
    cd fiends_and_fortune
    cmake -S . -B tmp
    cmake --build tmp --target all test

This repository contains a copy of the source code for [cJSON 1.7.16][34] and 
[tvision commit 9c9608e dated 2023-05-23][35].

[31]: https://cmake.org
[32]: https://libbsd.freedesktop.org/
[33]: https://www.nico.schottelius.org/software/gpm/
[34]: https://github.com/DaveGamble/cJSON
[35]: https://github.com/magiblot/tvision


## Build System

_Fiends And Fortune_ uses [Cmake][41] as a build system, [GitHub Actions][42] 
for continuous integration, and [Codecov.io][43] for code coverage reporting.

### Build Options

To build with the [AddressSanitizer][44] enabled, set the `ADDRESS_SANITIZER`
option to `ON`.

    cmake -S . -B tmp -DADDRESS_SANITIZER=ON

Set the `COVERAGE` option to `ON` to generate coverage files.

    cmake -S . -B tmp -DCOVERAGE=ON

Set the `WALL` option to `ON` turns on [additional warnings][46] using the 
`-Wall` compiler flag and also treats warnings as errors.  `WALL` is off by
default but is recommended for development and integration builds.

    cmake -S . -B tmp -DWALL=ON

Set the `GRAPH_TARGETS` option to `ON` to visualize the relationships between
Cmake build targets.  This will generate the files `fnf_targets.dot` and
`fnf_targets.png` in the `tmp` directory.  [Graphviz][47] must be installed to
use this option.

    cmake -S . -B tmp -DGRAPH_TARGETS=ON

[41]: https://cmake.org
[42]: https://github.com/donmccaughey/fiends_and_fortune/actions/workflows/tests.yml
[43]: https://codecov.io/gh/donmccaughey/fiends_and_fortune
[44]: https://clang.llvm.org/docs/LeakSanitizer.html
[45]: https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer
[46]: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options
[47]: https://graphviz.org


## License

_Fiends And Fortune_ is available under a BSD-style license.  See the
[`LICENSE`][51] file for details.  The [_cJSON_ library][52] is available under 
an [MIT license][53].  The code in the [_tvision_ library][54] is available
under a [variety of permissive licenses][55].

[51]: https://github.com/donmccaughey/fiends_and_fortune/blob/main/LICENSE
[52]: https://github.com/donmccaughey/fiends_and_fortune/blob/main/libs/cJSON
[53]: https://github.com/donmccaughey/fiends_and_fortune/blob/main/libs/cJSON/LICENSE
[54]: https://github.com/donmccaughey/fiends_and_fortune/blob/main/libs/tvision
[55]: https://github.com/donmccaughey/fiends_and_fortune/blob/main/libs/tvision/COPYRIGHT
