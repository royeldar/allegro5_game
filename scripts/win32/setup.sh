#!/bin/sh

set -e

docker build -f Dockerfile.base -t game-base-win32 .
