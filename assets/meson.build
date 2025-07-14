logo = custom_target('logo',
  input : files('logo.png'),
  output : 'logo.h',
  command : ['xxd', '-i', '-n', 'logo', '@INPUT@'],
  capture : true,
  install : false
)

font = custom_target('font',
  input : files('font.ttf'),
  output : 'font.c',
  command : ['sh', '-c', 'echo \'#include "common.h"\' && xxd -i -n font @INPUT0@'],
  capture : true,
  install : false
)
