project('mint', 'c',
  version : '0.3',
  default_options : [
    'c_std=c11',
    'optimization=3',
    'strip=true',
    'buildtype=release'
  ]
)

cairo_dep = dependency('cairo', required : true)
minui_dep = dependency('minui', required : true)

srcs = [
  'src/main.c',
  'src/battery.c',
  'src/splash.c',
  'src/text.c',
]

executable('mint', srcs,
  include_directories : include_directories('.'),
  dependencies : [cairo_dep, minui_dep]
)
