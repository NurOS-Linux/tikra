project(
    'aether-tikra',
    'cpp',
    version: '1.0.0',
    default_options: [
        'cpp_std=c++17',
        'warning_level=2',
        'buildtype=debugoptimized'
    ]
)

# Qt6 dependencies
qt6 = import('qt6')
qt6_deps = [
    dependency('qt6', modules: ['Core', 'Widgets', 'Gui'])
]

# Include directories
inc_dirs = include_directories('include')

# Application sources
tikra_sources = files(
    'src/main.cpp',
    'src/tikra.cpp'
)

tikra_headers = files(
    'include/tikra.hpp'
)

# MOC processing
tikra_moc = qt6.compile_moc(
    headers: tikra_headers,
    dependencies: qt6_deps,
    include_directories: inc_dirs
)

# Main executable
executable(
    'aether-tikra',
    tikra_sources,
    tikra_moc,
    include_directories: inc_dirs,
    dependencies: qt6_deps,
    install: true
)

# Build summary
summary({
    'Project': meson.project_name(),
    'Version': meson.project_version(),
    'Qt6 version': qt6_deps[0].version(),
}, section: 'Aether Tikra')