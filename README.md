# Game

## Brief description

This is a simple game framework built using Allegro5.

## Building instructions

### Native build (for Linux)

Building on a Linux distribution is pretty easy, simply run

```sh
meson build
meson compile -C build
```

However, there might be dependency issues when trying to run the binary on a different pc.

Therefore, one can build an AppImage, by running the `build.sh` script.

Finally, one can also build this AppImage using the supplied docker image.

#### Docker build (for Linux)

Build the docker image and run the container using the following command:

```sh
docker run -it --rm -v $(pwd)/bin:/root/bin --device /dev/fuse --cap-add SYS_ADMIN $(docker build -q .)
```

### Cross build (for Windows)

Building on a Linux distribution for Windows is done using MinGW; simply run

```sh
meson --cross-file win32/x86_64-w64-mingw32.txt build-win32
meson compile -C build-win32
```

One can build a zip archive by running the `build-win32.sh` script.

Finally, this can be done using a docker as well.

#### Docker build (for Windows cross-compilation)

Build the docker image and run the container using the following command:

```sh
docker run -it --rm -v $(pwd)/bin-win32:/root/bin-win32 $(docker build -f Dockerfile.win32 -q .)
```
