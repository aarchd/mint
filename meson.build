project('mint', 'c',
  version : '0.8',
  default_options : [
    'c_std=c11',
    'optimization=3',
    'strip=true',
    'buildtype=release'
  ]
)

cairo_dep = dependency('cairo', required : true)
minui_dep = dependency('minui', required : true)
freetype_dep = dependency('freetype2', required : true)

srcs = [
  'src/main.c',
  'src/battery.c',
  'src/splash.c',
  'src/text.c',
]

logo = custom_target('logo',
  input : files('assets/logo.png'),
  output : 'logo.h',
  command : ['xxd', '-i', '-n', 'logo', '@INPUT@'],
  capture : true,
  install : false
)

srcs += logo

font = custom_target('font',
  input : files('assets/font.ttf'),
  output : 'font.c',
  command : ['sh', '-c', 'echo \'#include "common.h"\' && xxd -i -n font @INPUT0@'],
  capture : true,
  install : false
)

srcs += font

executable('mint', srcs,
  include_directories : include_directories('src/.'),
  dependencies : [cairo_dep, minui_dep, freetype_dep],
  install : true
)

systemd_dep = dependency('systemd', required: false)

if systemd_dep.found()
  systemd_unit_dir = systemd_dep.get_variable('systemdsystemunitdir',
    pkgconfig_define: ['prefix', get_option('prefix')],
  )
  message('Installing systemd service to: ' + systemd_unit_dir)
  install_data('systemd-units/mint-splash.service',
    install_dir: systemd_unit_dir,
    install_tag: 'systemd'
  )
else
  message('Systemd support not found, skipping service installation.')
endif
