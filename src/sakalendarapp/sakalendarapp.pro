TEMPLATE = subdirs

SUBDIRS = plugins gui app

icons.files = icons/sakalendar.png
icons.path = $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/share/icons
INSTALLS = icons

OTHER_FILES = \
    TODO
