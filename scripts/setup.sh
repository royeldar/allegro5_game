#!/bin/sh

set -e

flatpak remote-add --if-not-exists flathub https://dl.flathub.org/repo/flathub.flatpakrepo
flatpak install -y flathub org.freedesktop.Platform//25.08
flatpak install -y flathub org.freedesktop.Sdk//25.08
