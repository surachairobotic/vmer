QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cReport.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    cReport.h \
    mainwindow.h \
    wkhtmltox/image.h \
    wkhtmltox/pdf.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += ../qpdflib

CONFIG(debug, debug|release) {
    win32:LIBS += -L$$OUT_PWD/../qpdflib/debug
} else {
    win32:LIBS += -L$$OUT_PWD/../qpdflib/release
}
win32:LIBS += qpdf.lib
unix:LIBS += -L$$OUT_PWD/../qpdflib
unix:LIBS += -lqpdf

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$PWD/wkhtmltox/ #-llibwkhtmltox.a
LIBS += /LINK libwkhtmltox.a
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/wkhtmltox/ -lwkhtmltox
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/wkhtmltox/ -lwkhtmltox
#else:unix: LIBS += -L$$PWD/wkhtmltox/ -lwkhtmltox

INCLUDEPATH += $$PWD/wkhtmltox
DEPENDPATH += $$PWD/wkhtmltox

PRE_TARGETDEPS += $$PWD/wkhtmltox/libwkhtmltox.a

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/wkhtmltox/libwkhtmltox.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/wkhtmltox/libwkhtmltox.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/wkhtmltox/libwkhtmltox.a
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/wkhtmltox/libwkhtmltox.a
#else:unix: PRE_TARGETDEPS += $$PWD/wkhtmltox/libwkhtmltox.a
