#!/bin/sh

set -e

meson build
meson compile -C build
./linuxdeploy-x86_64.AppImage \
    --appdir AppDir \
    --executable build/game \
    --desktop-file misc/game.desktop \
    --icon-file misc/game.svg \
    --custom-apprun misc/AppRun
ARCH=x86_64 ./appimagetool-x86_64.AppImage AppDir
mv game-x86_64.AppImage bin/
