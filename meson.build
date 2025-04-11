project('game', 'c')

cmake = import('cmake')

liballegro5_opts = cmake.subproject_options()
liballegro5_opts.add_cmake_defines({'SHARED': false})

liballegro5_proj = cmake.subproject('liballegro5', options: liballegro5_opts)

liballegro5_deps = [
  liballegro5_proj.dependency('allegro'),
  liballegro5_proj.dependency('allegro_primitives'),
  liballegro5_proj.dependency('allegro_image'),
  liballegro5_proj.dependency('allegro_font'),
  liballegro5_proj.dependency('allegro_audio'),
  liballegro5_proj.dependency('allegro_acodec'),
  liballegro5_proj.dependency('allegro_ttf'),
  liballegro5_proj.dependency('allegro_color'),
  liballegro5_proj.dependency('allegro_memfile'),
  liballegro5_proj.dependency('allegro_physfs'),
  liballegro5_proj.dependency('allegro_dialog'),
  liballegro5_proj.dependency('allegro_video'),
  liballegro5_proj.dependency('allegro_main'),
]

subdir('src')

executable(
  'game',
  sources: sources,
  dependencies: liballegro5_deps,
)
