QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainNewProj.cpp \
    mainTest.cpp \
    mainDisplayTree.cpp \
    mainNewElementWindow.cpp \
    mainNewModelWindow.cpp \
    mainNewShopWindow.cpp \
    mainNewRouteWindow.cpp \
    mainNewMachineWindow.cpp \
    mainRightClickMenu.cpp \
    cRoute.cpp \
    cDB.cpp \
    cDBQuery.cpp \
    cDBLink.cpp \
    cDBPrint.cpp \
    cDBInsert.cpp \
    cDBDelete.cpp \
    cDBUpdate.cpp \
    cModel.cpp \
    cNewProjectDialog.cpp \
    cNewElementWindow.cpp \
    cNewModelWindow.cpp \
    cNewShopWindow.cpp \
    cNewRouteWindow.cpp \
    cNewMachineWindow.cpp \
    commonFunction.cpp \
    cElementProperties.cpp \
    cReport.cpp

HEADERS += \
    mainwindow.h \
    cRoute.h \
    cDB.h \
    cModel.h \
    cNewProjectDialog.h \
    cNewElementWindow.h \
    cNewModelWindow.h \
    cNewShopWindow.h \
    cNewRouteWindow.h \
    cNewMachineWindow.h \
    cDirHandle.h \
    commonFunction.h \
    cElementProperties.h \
    cReport.h \
    wkhtmltox/image.h \
    wkhtmltox/pdf.h

FORMS += \
    mainwindow.ui \
    cNewProjectDialog.ui \
    cNewElementWindow.ui \
    cNewModelWindow.ui \
    cNewShopWindow.ui \
    cNewRouteWindow.ui \
    cNewMachineWindow.ui

#cNewElement.h
#cNewElement.cpp
#cNewElement.ui

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
