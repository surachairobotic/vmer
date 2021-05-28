cmake_minimum_required(VERSION 3.5)

project(vmer_desktop LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        mainNewProj.cpp
        mainTest.cpp
        mainDisplayTree.cpp
        mainNewElementWindow.cpp
        mainNewModelWindow.cpp
        mainNewShopWindow.cpp
        mainNewRouteWindow.cpp
        mainRightClickMenu.cpp
        cRoute.h
        cRoute.cpp
        cDB.h
        cDB.cpp
        cDBQuery.cpp
        cDBLink.cpp
        cDBPrint.cpp
        cDBInsert.cpp
        cDBDelete.cpp
        cDBUpdate.cpp
        cModel.h
        cModel.cpp
        cNewProjectDialog.h
        cNewProjectDialog.cpp
        cNewProjectDialog.ui
        cNewElementWindow.h
        cNewElementWindow.cpp
        cNewElementWindow.ui
        cNewModelWindow.h
        cNewModelWindow.cpp
        cNewModelWindow.ui
        cNewShopWindow.h
        cNewShopWindow.cpp
        cNewShopWindow.ui
        cNewRouteWindow.h
        cNewRouteWindow.cpp
        cNewRouteWindow.ui
        #cNewElement.h
        #cNewElement.cpp
        #cNewElement.ui
        cDirHandle.h
)

qt5_add_resources(PROJECT_SOURCES resources/resourcesFile.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(vmer_desktop
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(vmer_desktop SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(vmer_desktop
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(vmer_desktop PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)
