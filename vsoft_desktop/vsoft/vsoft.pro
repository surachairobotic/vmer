QT       += core gui sql network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cReportWindow.cpp \
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
    cReport.cpp \
    cHTTPRequest.cpp \
    cHTTPRequestGUI.cpp

HEADERS += \
    cReportWindow.h \
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
    wkhtmltox/pdf.h \
    cHTTPRequest.h \
    cHTTPRequestGUI.h \
    cListWidgetItemDevice.h \
    ziplib/ZipArchive.h \
    ziplib/ZipArchiveEntry.h \
    ziplib/ZipFile.h \
    ziplib/compression/bzip2/bzip2_decoder.h \
    ziplib/compression/bzip2/bzip2_decoder_properties.h \
    ziplib/compression/bzip2/bzip2_encoder.h \
    ziplib/compression/bzip2/bzip2_encoder_properties.h \
    ziplib/compression/compression_interface.h \
    ziplib/compression/deflate/deflate_decoder.h \
    ziplib/compression/deflate/deflate_decoder_properties.h \
    ziplib/compression/deflate/deflate_encoder.h \
    ziplib/compression/deflate/deflate_encoder_properties.h \
    ziplib/compression/lzma/detail/lzma_alloc.h \
    ziplib/compression/lzma/detail/lzma_handle.h \
    ziplib/compression/lzma/detail/lzma_header.h \
    ziplib/compression/lzma/detail/lzma_in_stream.h \
    ziplib/compression/lzma/detail/lzma_out_stream.h \
    ziplib/compression/lzma/lzma_decoder.h \
    ziplib/compression/lzma/lzma_decoder_properties.h \
    ziplib/compression/lzma/lzma_encoder.h \
    ziplib/compression/lzma/lzma_encoder_properties.h \
    ziplib/compression/store/store_decoder.h \
    ziplib/compression/store/store_decoder_properties.h \
    ziplib/compression/store/store_encoder.h \
    ziplib/compression/store/store_encoder_properties.h \
    ziplib/detail/EndOfCentralDirectoryBlock.h \
    ziplib/detail/ZipCentralDirectoryFileHeader.h \
    ziplib/detail/ZipGenericExtraField.h \
    ziplib/detail/ZipLocalFileHeader.h \
    ziplib/extlibs/bzip2/bzlib.h \
    ziplib/extlibs/bzip2/bzlib_private.h \
    ziplib/extlibs/lzma/7z.h \
    ziplib/extlibs/lzma/7zAlloc.h \
    ziplib/extlibs/lzma/7zBuf.h \
    ziplib/extlibs/lzma/7zCrc.h \
    ziplib/extlibs/lzma/7zFile.h \
    ziplib/extlibs/lzma/7zVersion.h \
    ziplib/extlibs/lzma/Alloc.h \
    ziplib/extlibs/lzma/Bcj2.h \
    ziplib/extlibs/lzma/Bra.h \
    ziplib/extlibs/lzma/CpuArch.h \
    ziplib/extlibs/lzma/Delta.h \
    ziplib/extlibs/lzma/LzFind.h \
    ziplib/extlibs/lzma/LzFindMt.h \
    ziplib/extlibs/lzma/LzHash.h \
    ziplib/extlibs/lzma/Lzma2Dec.h \
    ziplib/extlibs/lzma/Lzma2Enc.h \
    ziplib/extlibs/lzma/Lzma86.h \
    ziplib/extlibs/lzma/LzmaDec.h \
    ziplib/extlibs/lzma/LzmaEnc.h \
    ziplib/extlibs/lzma/LzmaLib.h \
    ziplib/extlibs/lzma/MtCoder.h \
    ziplib/extlibs/lzma/Ppmd.h \
    ziplib/extlibs/lzma/Ppmd7.h \
    ziplib/extlibs/lzma/RotateDefs.h \
    ziplib/extlibs/lzma/Sha256.h \
    ziplib/extlibs/lzma/Threads.h \
    ziplib/extlibs/lzma/Types.h \
    ziplib/extlibs/lzma/Xz.h \
    ziplib/extlibs/lzma/XzCrc64.h \
    ziplib/extlibs/lzma/XzEnc.h \
    ziplib/extlibs/lzma/unix/7zBuf.h \
    ziplib/extlibs/lzma/unix/7zCrc.h \
    ziplib/extlibs/lzma/unix/7zVersion.h \
    ziplib/extlibs/lzma/unix/Aes.h \
    ziplib/extlibs/lzma/unix/Alloc.back3 \
    ziplib/extlibs/lzma/unix/Alloc.c.back \
    ziplib/extlibs/lzma/unix/Alloc.c.back2 \
    ziplib/extlibs/lzma/unix/Alloc.h \
    ziplib/extlibs/lzma/unix/Bra.h \
    ziplib/extlibs/lzma/unix/BwtSort.h \
    ziplib/extlibs/lzma/unix/CpuArch.h \
    ziplib/extlibs/lzma/unix/Delta.h \
    ziplib/extlibs/lzma/unix/HuffEnc.h \
    ziplib/extlibs/lzma/unix/LzFind.h \
    ziplib/extlibs/lzma/unix/LzFindMt.h \
    ziplib/extlibs/lzma/unix/LzHash.h \
    ziplib/extlibs/lzma/unix/Lzma2Dec.h \
    ziplib/extlibs/lzma/unix/Lzma2Enc.h \
    ziplib/extlibs/lzma/unix/LzmaDec.h \
    ziplib/extlibs/lzma/unix/LzmaEnc.h \
    ziplib/extlibs/lzma/unix/LzmaUtil/Lzma86Dec.h \
    ziplib/extlibs/lzma/unix/LzmaUtil/Lzma86Enc.h \
    ziplib/extlibs/lzma/unix/MtCoder.h \
    ziplib/extlibs/lzma/unix/Ppmd.h \
    ziplib/extlibs/lzma/unix/Ppmd7.h \
    ziplib/extlibs/lzma/unix/Ppmd8.h \
    ziplib/extlibs/lzma/unix/RotateDefs.h \
    ziplib/extlibs/lzma/unix/Sha256.h \
    ziplib/extlibs/lzma/unix/Sort.h \
    ziplib/extlibs/lzma/unix/Threads.h \
    ziplib/extlibs/lzma/unix/Types.h \
    ziplib/extlibs/lzma/unix/Xz.h \
    ziplib/extlibs/lzma/unix/XzCrc64.h \
    ziplib/extlibs/lzma/unix/XzEnc.h \
    ziplib/extlibs/zlib/crc32.h \
    ziplib/extlibs/zlib/deflate.h \
    ziplib/extlibs/zlib/gzguts.h \
    ziplib/extlibs/zlib/inffast.h \
    ziplib/extlibs/zlib/inffixed.h \
    ziplib/extlibs/zlib/inflate.h \
    ziplib/extlibs/zlib/inftrees.h \
    ziplib/extlibs/zlib/trees.h \
    ziplib/extlibs/zlib/zconf.h \
    ziplib/extlibs/zlib/zlib.h \
    ziplib/extlibs/zlib/zutil.h \
    ziplib/methods/Bzip2Method.h \
    ziplib/methods/DeflateMethod.h \
    ziplib/methods/ICompressionMethod.h \
    ziplib/methods/LzmaMethod.h \
    ziplib/methods/StoreMethod.h \
    ziplib/methods/ZipMethodResolver.h \
    ziplib/streams/compression_decoder_stream.h \
    ziplib/streams/compression_encoder_stream.h \
    ziplib/streams/crc32stream.h \
    ziplib/streams/memstream.h \
    ziplib/streams/nullstream.h \
    ziplib/streams/serialization.h \
    ziplib/streams/streambuffs/compression_decoder_streambuf.h \
    ziplib/streams/streambuffs/compression_encoder_streambuf.h \
    ziplib/streams/streambuffs/crc32_streambuf.h \
    ziplib/streams/streambuffs/mem_streambuf.h \
    ziplib/streams/streambuffs/null_streambuf.h \
    ziplib/streams/streambuffs/sub_streambuf.h \
    ziplib/streams/streambuffs/tee_streambuff.h \
    ziplib/streams/streambuffs/zip_crypto_streambuf.h \
    ziplib/streams/substream.h \
    ziplib/streams/teestream.h \
    ziplib/streams/zip_cryptostream.h \
    ziplib/utils/enum_utils.h \
    ziplib/utils/stream_utils.h \
    ziplib/utils/time_utils.h


FORMS += \
    cReportWindow.ui \
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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ziplib/lib/release/ -lbzip2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ziplib/lib/debug/ -lbzip2

INCLUDEPATH += $$PWD/''
DEPENDPATH += $$PWD/''

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/release/libbzip2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/debug/libbzip2.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/release/bzip2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/debug/bzip2.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ziplib/lib/release/ -llzma
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ziplib/lib/debug/ -llzma

INCLUDEPATH += $$PWD/ziplib/lib/debug
DEPENDPATH += $$PWD/ziplib/lib/debug

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/release/liblzma.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/debug/liblzma.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/release/lzma.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/debug/lzma.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ziplib/lib/release/ -lZipLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ziplib/lib/debug/ -lZipLib

INCLUDEPATH += $$PWD/ziplib/lib/debug
DEPENDPATH += $$PWD/ziplib/lib/debug

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/release/libZipLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/debug/libZipLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/release/ZipLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/debug/ZipLib.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ziplib/lib/release/ -lzlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ziplib/lib/debug/ -lzlib

INCLUDEPATH += $$PWD/ziplib/lib/debug
DEPENDPATH += $$PWD/ziplib/lib/debug

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/release/libzlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/debug/libzlib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/release/zlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ziplib/lib/debug/zlib.lib

RESOURCES += \
    vsoft_resource.qrc
