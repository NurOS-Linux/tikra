project('tikra',
    ['vala', 'c'],
    version: '1.0.0',
    license: 'GPL-3.0',
    meson_version: '>= 0.56.0'
)

gnome = import('gnome')
i18n = import('i18n')

vala_version_required = '0.56.0'
vala = meson.get_compiler('vala')

if not vala.version().version_compare('>=' + vala_version_required)
    error('Vala version @0@ or newer is required.'.format(vala_version_required))
endif

glib_required = '2.66.0'
gtk_required = '4.10.0'
libadwaita_required = '1.4.0'

glib_dep = dependency('glib-2.0', version: '>=' + glib_required)
gobject_dep = dependency('gobject-2.0', version: '>=' + glib_required)
gio_dep = dependency('gio-2.0', version: '>=' + glib_required)
gtk_dep = dependency('gtk4', version: '>=' + gtk_required)
libadwaita_dep = dependency('libadwaita-1', version: '>=' + libadwaita_required)

app_id = 'org.aether.tikra'

add_project_arguments(
    '--target-glib=@0@'.format(glib_required),
    language: 'vala'
)

subdir('data')
subdir('src')

gnome.post_install(
    glib_compile_schemas: true,
    gtk_update_icon_cache: true,
    update_desktop_database: true,
)