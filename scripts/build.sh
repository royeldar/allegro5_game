#!/bin/sh

set -e

rm -rf repo

flatpak-builder --force-clean --repo=repo builddir io.github.royeldar.game.yml
flatpak build-bundle repo game.flatpak io.github.royeldar.game
