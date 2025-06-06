#!/bin/sh

set -e

meson --cross-file win32/x86_64-w64-mingw32.txt build-win32
meson compile -C build-win32

EXECUTABLE="build-win32/game.exe"
GFX_DIR="misc/gfx"
SFX_DIR="misc/sfx"
ZIP="bin-win32/game.zip"

mkdir tmp
cp "$EXECUTABLE" tmp
cp -r "$GFX_DIR" "$SFX_DIR" tmp
rm tmp/*/.gitattributes

cd tmp
zip -r ../"$ZIP" .

cd ..
rm -r tmp
