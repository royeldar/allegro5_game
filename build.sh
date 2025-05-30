#!/bin/sh

set -e

meson build
meson compile -C build
./linuxdeploy-x86_64.AppImage \
    --appdir AppDir \
    --executable build/game \
    --desktop-file misc/game.desktop \
    --icon-file misc/game.svg \
    --custom-apprun misc/AppRun \
    --output appimage
mv game-x86_64.AppImage bin/
