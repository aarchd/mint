systemd_dep = dependency('systemd', required: false)

if systemd_dep.found()
  systemd_unit_dir = systemd_dep.get_variable('systemdsystemunitdir',
    pkgconfig_define: ['prefix', get_option('prefix')],
  )
  message('Installing systemd service to: ' + systemd_unit_dir)
  install_data('mint-splash.service',
    install_dir: systemd_unit_dir,
    install_tag: 'systemd'
  )
else
  message('Systemd support not found, skipping service installation.')
endif
