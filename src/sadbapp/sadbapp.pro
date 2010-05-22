TEMPLATE = subdirs

SUBDIRS = database dk4db sorokovnikdb gui app

icons.files = icons/sadb.png
icons.path = $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/share/icons
INSTALLS = icons

OTHER_FILES = \
    README
