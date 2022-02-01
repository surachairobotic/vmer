TEMPLATE = subdirs

SUBDIRS += \
    qpdflib \
    vsoft

vsoft.depends = qpdflib
