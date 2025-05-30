#!/bin/sh

set -e

meson build
meson compile -C build
./linuxdeploy-x86_64.AppImage \
    --appdir AppDir \
    --library /usr/lib/libc.so.6 \
    --library /usr/lib/libm.so.6 \
    --library /usr/lib/libpthread.so.0 \
    --library /usr/lib/librt.so.1 \
    --library /usr/lib/libstdc++.so.6 \
    --library /usr/lib/libz.so.1 \
    --library /usr/lib/libfontconfig.so.1 \
    --library /usr/lib/libfreetype.so.6 \
    --library /usr/lib/libfribidi.so.0 \
    --library /usr/lib/libharfbuzz.so.0 \
    --exclude-library libwayland-* \
    --exclude-library libgdk-3.so.0 \
    --exclude-library libgtk-3.so.0 \
    --executable build/game \
    --desktop-file misc/game.desktop \
    --icon-file misc/game.svg \
    --custom-apprun misc/AppRun
mkdir AppDir/lib64
cp /lib64/ld-linux-x86-64.so.2 AppDir/lib64
ARCH=x86_64 ./appimagetool-x86_64.AppImage AppDir
mv game-x86_64.AppImage bin/
