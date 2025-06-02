#!/bin/sh

set -e

meson build
meson compile -C build

SHARED_LIBS=
SHARED_LIBS="$SHARED_LIBS /usr/lib/libc.so.6"
SHARED_LIBS="$SHARED_LIBS /usr/lib/libm.so.6"
SHARED_LIBS="$SHARED_LIBS /usr/lib/libpthread.so.0"
SHARED_LIBS="$SHARED_LIBS /usr/lib/librt.so.1"
SHARED_LIBS="$SHARED_LIBS /usr/lib/libstdc++.so.6"
SHARED_LIBS="$SHARED_LIBS /usr/lib/libz.so.1"
SHARED_LIBS="$SHARED_LIBS /usr/lib/libfontconfig.so.1"
SHARED_LIBS="$SHARED_LIBS /usr/lib/libfreetype.so.6"
SHARED_LIBS="$SHARED_LIBS /usr/lib/libfribidi.so.0"
SHARED_LIBS="$SHARED_LIBS /usr/lib/libharfbuzz.so.0"

EXCLUDE_LIBS=
EXCLUDE_LIBS="$EXCLUDE_LIBS libwayland-*"
EXCLUDE_LIBS="$EXCLUDE_LIBS libgdk-3.so.0"
EXCLUDE_LIBS="$EXCLUDE_LIBS libgtk-3.so.0"

EXECUTABLE="build/game"
DESKTOP="misc/game.desktop"
ICON="misc/game.svg"
APPRUN="misc/AppRun"

INTERP="/lib64/ld-linux-x86-64.so.2"

GFX_DIR="misc/gfx"
SFX_DIR="misc/sfx"

APPIMAGE="game-x86_64.AppImage"

DEPENDENCIES="$(ldd "$EXECUTABLE" | cut -d" "  -f3)"
DEPENDENCIES="$DEPENDENCIES /usr/lib/libpthread.so.0 /usr/lib/librt.so.1"

EXTRA_LIBS=""

for lib in $SHARED_LIBS; do
    case "$DEPENDENCIES" in
    *"$lib"*)
        EXTRA_LIBS="$EXTRA_LIBS --library $lib"
        ;;
    esac
done

for lib in $EXCLUDE_LIBS; do
    EXTRA_LIBS="$EXTRA_LIBS --exclude-library $lib"
done

APPDIR="AppDir"

./linuxdeploy-x86_64.AppImage \
    --appdir "$APPDIR" \
    $EXTRA_LIBS \
    --executable "$EXECUTABLE" \
    --desktop-file "$DESKTOP" \
    --icon-file "$ICON" \
    --custom-apprun "$APPRUN"

mkdir -p "$APPDIR$(dirname "$INTERP")"
cp "$INTERP" "$APPDIR$INTERP"

cp -r "$GFX_DIR" "$SFX_DIR" "$APPDIR"

ARCH=x86_64 ./appimagetool-x86_64.AppImage "$APPDIR"
mv "$APPIMAGE" bin/
