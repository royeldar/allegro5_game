#!/bin/sh

set -e

meson --cross-file win32/x86_64-w64-mingw32.txt build-win32
meson compile -C build-win32

EXECUTABLE="build-win32/game.exe"
ZIP="bin-win32/game.zip"

mkdir tmp
cp "$EXECUTABLE" tmp

cd tmp
zip -r ../"$ZIP" .

cd ..
rm -r tmp
