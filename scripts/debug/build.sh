#!/bin/sh

set -e

flatpak build-bundle --runtime repo game.Debug.flatpak io.github.royeldar.game.Debug
