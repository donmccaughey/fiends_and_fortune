# SourceHut Builds

Since SourceHut no longer directly supports running builds from GitHub, I've
removed the SourceHut build files and put their content here for reference.

## Debian Stable

Path: `.builds/debian_stable.yml`

    image: debian/stable
    packages:
        - cmake
        - libbsd-dev
        - ncurses-dev
    sources:
        - https://github.com/donmccaughey/fiends_and_fortune
    tasks:
        - setup: |
            cd fiends_and_fortune
            cmake -S . -B tmp -DADDRESS_SANITIZER=ON -DWALL=ON
        - build: |
            cd fiends_and_fortune
            cmake --build tmp --target all
            cd tmp
            ctest --output-on-failure
    triggers:
        - action: email
          condition: failure
          to: Don McCaughey <don@donm.cc>


## FreeBSD Latest

Path: `.builds/freebsd_latest.yml`

    image: freebsd/latest
    packages:
        - cmake
    sources:
        - https://github.com/donmccaughey/fiends_and_fortune
    tasks:
        - setup: |
            cd fiends_and_fortune
            cmake -S . -B tmp -DWALL=ON
        - build: |
            cd fiends_and_fortune
            cmake --build tmp --target all
            cd tmp
            ctest --output-on-failure
    triggers:
        - action: email
          condition: failure
          to: Don McCaughey <don@donm.cc>


## OpenBSD Latest

Path: `.builds/openbsd_latest.yml`

    image: openbsd/latest
    packages:
        - cmake
    sources:
        - https://github.com/donmccaughey/fiends_and_fortune
    tasks:
        - setup: |
            cd fiends_and_fortune
            CC=cc cmake -S . -B tmp -DWALL=ON
        - build: |
            cd fiends_and_fortune
            cmake --build tmp --target all
            cd tmp
            ctest --output-on-failure
    triggers:
        - action: email
          condition: failure
          to: Don McCaughey <don@donm.cc>
