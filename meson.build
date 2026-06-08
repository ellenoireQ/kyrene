project('kyrene', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3', 
    'cpp_std=c++17'
  ])

gtkmm_dep = dependency('gtkmm-4.0')
gnome = import('gnome')

resources = gnome.compile_resources('kyrene-resources',
  'resources.gresource.xml',
  source_dir : '.',
  c_name : 'kyrene_resources')

executable('kyrene',
  ['main.cpp', resources],
  dependencies : gtkmm_dep
)