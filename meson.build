project('kyrene', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3', 
    'cpp_std=c++20'
  ])

class_dir = include_directories('class')
utils_dir = include_directories('utils')
core_dir = include_directories('core')
monitor_dir = include_directories('monitor')

class_dep = declare_dependency(
  include_directories: class_dir
)

nlohmann_dep = dependency('nlohmann_json', required : true)

utils_dep = declare_dependency(
  include_directories: [utils_dir, core_dir, monitor_dir],
  dependencies: [nlohmann_dep]
)

gtkmm_dep = dependency('gtkmm-4.0')
gnome = import('gnome')

resources = gnome.compile_resources('kyrene-resources',
  'resources.gresource.xml',
  source_dir : '.',
  c_name : 'kyrene_resources')

executable('kyrene',
  ['main.cpp', 'monitor/cpu_monitor.cpp', resources],
  dependencies : [gtkmm_dep, class_dep, utils_dep]
)