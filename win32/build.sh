#!/bin/sh

set -e

meson --cross-file win32/x86_64-w64-mingw32.txt --prefix "$PWD/install" build src
meson compile -C build
meson install -C build
