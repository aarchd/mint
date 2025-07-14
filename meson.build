project('mint', 'c',
  version : '0.9',
  default_options : [
    'warning_level=3',
    'werror=true',
  ]
)

cairo_dep = dependency('cairo', required : true)
minui_dep = dependency('minui', required : true)
freetype_dep = dependency('freetype2', required : true)

subdir('src')
subdir('assets')
subdir('systemd-units')

srcs = src_files + [logo, font]

executable('mint', srcs,
  include_directories : inc,
  dependencies : [cairo_dep, minui_dep, freetype_dep],
  install : true
)
