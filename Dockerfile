FROM archlinux:latest

RUN pacman --noconfirm -Sy && \
    pacman --noconfirm -S allegro desktop-file-utils fuse gcc meson pkgconfig squashfs-tools && \
    pacman --noconfirm -Scc && \
    rm -rf /var/lib/pacman/sync/*

WORKDIR /root
COPY . ./
RUN mkdir bin
ADD https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage linuxdeploy-x86_64.AppImage
ADD https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage appimagetool-x86_64.AppImage
RUN chmod +x linuxdeploy-x86_64.AppImage appimagetool-x86_64.AppImage

RUN ./linuxdeploy-x86_64.AppImage --appimage-extract
RUN rm squashfs-root/usr/bin/strip
RUN squashfs-root/plugins/linuxdeploy-plugin-appimage/appimagetool-prefix/usr/bin/appimagetool -gn squashfs-root/
RUN rm -r squashfs-root

ENTRYPOINT ["./build.sh"]
