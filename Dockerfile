FROM archlinux:latest

WORKDIR /root
COPY . ./
RUN mkdir bin
ADD https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage linuxdeploy-x86_64.AppImage
RUN chmod +x linuxdeploy-x86_64.AppImage

RUN pacman --noconfirm -Syu
RUN pacman --noconfirm -S desktop-file-utils squashfs-tools

RUN ./linuxdeploy-x86_64.AppImage --appimage-extract
RUN rm squashfs-root/usr/bin/strip
RUN squashfs-root/plugins/linuxdeploy-plugin-appimage/appimagetool-prefix/usr/bin/appimagetool -gn squashfs-root/
RUN rm -r squashfs-root

RUN pacman --noconfirm -S fuse
RUN pacman --noconfirm -S gcc meson pkgconf
RUN pacman --noconfirm -S allegro

ENTRYPOINT ["./build.sh"]
