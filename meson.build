project('game', 'c')

cc = meson.get_compiler('c')

liballegro5_deps = [
  dependency('allegro-5'),
  dependency('allegro_primitives-5'),
  dependency('allegro_image-5'),
  dependency('allegro_font-5'),
  dependency('allegro_audio-5'),
  dependency('allegro_acodec-5'),
  dependency('allegro_ttf-5'),
  dependency('allegro_color-5'),
  dependency('allegro_memfile-5'),
  dependency('allegro_physfs-5'),
  dependency('allegro_dialog-5'),
  dependency('allegro_video-5'),
  dependency('allegro_main-5'),
]

subdir('src')

executable(
  'game',
  sources: sources,
  dependencies: liballegro5_deps,
)
