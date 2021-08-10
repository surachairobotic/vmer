TEMPLATE = subdirs

SUBDIRS = qpdflib\
          report

report.depends = qpdflib
