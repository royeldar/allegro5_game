project('game', 'c')

cc = meson.get_compiler('c')

liballegro5_deps = [
  dependency('allegro-static-5', static: true),
  dependency('allegro_primitives-static-5', static: true),
  dependency('allegro_image-static-5', static: true),
  dependency('allegro_font-static-5', static: true),
  dependency('allegro_audio-static-5', static: true),
  dependency('allegro_acodec-static-5', static: true),
  dependency('allegro_ttf-static-5', static: true),
  dependency('allegro_color-static-5', static: true),
  dependency('allegro_memfile-static-5', static: true),
  dependency('allegro_physfs-static-5', static: true),
  dependency('allegro_dialog-static-5', static: true),
  dependency('allegro_video-static-5', static: true),
  dependency('allegro_main-static-5', static: true),
  cc.find_library('dl', static: true),
  cc.find_library('stdc++', static: true),
  dependency('xrender', static: true),
  dependency('xfixes', static: true),
]

subdir('src')

executable(
  'game',
  sources: sources,
  dependencies: liballegro5_deps,
)
