    QT -= gui
    win32:QT += axcontainer

TEMPLATE = lib
DEFINES += PDFTOHTML_LIBRARY

CONFIG += c++20
unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
}

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-parameter

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AcroForm.cc \
    Annot.cc \
    Array.cc \
    BuiltinFont.cc \
    BuiltinFontTables.cc \
    CMap.cc \
    Catalog.cc \
    CharCodeToUnicode.cc \
    Decrypt.cc \
    Dict.cc \
    DisplayState.cc \
    Error.cc \
    FontEncodingTables.cc \
    Function.cc \
    Gfx.cc \
    GfxFont.cc \
    GfxState.cc \
    GlobalParams.cc \
    HTMLGen.cc \
    ImageOutputDev.cc \
    JArithmeticDecoder.cc \
    JBIG2Stream.cc \
    JPXStream.cc \
    Lexer.cc \
    Link.cc \
    NameToCharCode.cc \
    Object.cc \
    OptionalContent.cc \
    Outline.cc \
    OutputDev.cc \
    PDF417Barcode.cc \
    PDFCore.cc \
    PDFDoc.cc \
    PDFDocEncoding.cc \
    PSOutputDev.cc \
    PSTokenizer.cc \
    Page.cc \
    Parser.cc \
    PreScanOutputDev.cc \
    SecurityHandler.cc \
    ShadingImage.cc \
    SplashOutputDev.cc \
    Stream.cc \
    TextOutputDev.cc \
    TextString.cc \
    TileCache.cc \
    TileCompositor.cc \
    TileMap.cc \
    UTF8.cc \
    UnicodeMap.cc \
    UnicodeRemapping.cc \
    UnicodeTypeTable.cc \
    WebFont.cc \
    XFAScanner.cc \
    XRef.cc \
    Zoox.cc \
    fofi/FoFiBase.cc \
    fofi/FoFiEncodings.cc \
    fofi/FoFiIdentifier.cc \
    fofi/FoFiTrueType.cc \
    fofi/FoFiType1.cc \
    fofi/FoFiType1C.cc \
    goo/FixedPoint.cc \
    goo/GHash.cc \
    goo/GList.cc \
    goo/GString.cc \
    goo/Trace.cc \
    goo/gfile.cc \
    goo/gmem.cc \
    goo/gmempp.cc \
    goo/parseargs.c \
    pdfdetach.cc \
    pdftohtml.cpp \
    splash/Splash.cc \
    splash/SplashBitmap.cc \
    splash/SplashClip.cc \
    splash/SplashFTFont.cc \
    splash/SplashFTFontEngine.cc \
    splash/SplashFTFontFile.cc \
    splash/SplashFont.cc \
    splash/SplashFontEngine.cc \
    splash/SplashFontFile.cc \
    splash/SplashFontFileID.cc \
    splash/SplashPath.cc \
    splash/SplashPattern.cc \
    splash/SplashScreen.cc \
    splash/SplashState.cc \
    splash/SplashXPath.cc \
    splash/SplashXPathScanner.cc

HEADERS += \
    AcroForm.h \
    Annot.h \
    Array.h \
    BuiltinFont.h \
    BuiltinFontTables.h \
    CMap.h \
    Catalog.h \
    CharCodeToUnicode.h \
    CharTypes.h \
    CompactFontTables.h \
    Decrypt.h \
    Dict.h \
    DisplayState.h \
    Error.h \
    ErrorCodes.h \
    FontEncodingTables.h \
    Function.h \
    Gfx.h \
    GfxFont.h \
    GfxState.h \
    GlobalParams.h \
    HTMLGen.h \
    ImageOutputDev.h \
    JArithmeticDecoder.h \
    JBIG2Stream.h \
    JPXStream.h \
    Lexer.h \
    Link.h \
    NameToCharCode.h \
    NameToUnicodeTable.h \
    Object.h \
    OptionalContent.h \
    Outline.h \
    OutputDev.h \
    PDF417Barcode.h \
    PDFCore.h \
    PDFDoc.h \
    PDFDocEncoding.h \
    PSOutputDev.h \
    PSTokenizer.h \
    Page.h \
    Parser.h \
    PreScanOutputDev.h \
    SecurityHandler.h \
    ShadingImage.h \
    SplashOutputDev.h \
    Stream-CCITT.h \
    Stream.h \
    TextOutputDev.h \
    TextString.h \
    TileCache.h \
    TileCompositor.h \
    TileMap.h \
    UTF8.h \
    UnicodeMap.h \
    UnicodeMapTables.h \
    UnicodeRemapping.h \
    UnicodeTypeTable.h \
    WebFont.h \
    XFAScanner.h \
    XRef.h \
    Zoox.h \
    aconf.h \
    aconf2.h \
    config.h \
    fofi/FoFiBase.h \
    fofi/FoFiEncodings.h \
    fofi/FoFiIdentifier.h \
    fofi/FoFiTrueType.h \
    fofi/FoFiType1.h \
    fofi/FoFiType1C.h \
    goo/FixedPoint.h \
    goo/GHash.h \
    goo/GList.h \
    goo/GMutex.h \
    goo/GString.h \
    goo/Trace.h \
    goo/gfile.h \
    goo/gmem.h \
    goo/gmempp.h \
    goo/gtypes.h \
    goo/parseargs.h \
    pdftohtml_global.h \
    pdftohtml.h \
    splash/Splash.h \
    splash/SplashBitmap.h \
    splash/SplashClip.h \
    splash/SplashErrorCodes.h \
    splash/SplashFTFont.h \
    splash/SplashFTFontEngine.h \
    splash/SplashFTFontFile.h \
    splash/SplashFont.h \
    splash/SplashFontEngine.h \
    splash/SplashFontFile.h \
    splash/SplashFontFileID.h \
    splash/SplashGlyphBitmap.h \
    splash/SplashMath.h \
    splash/SplashPath.h \
    splash/SplashPattern.h \
    splash/SplashScreen.h \
    splash/SplashState.h \
    splash/SplashTypes.h \
    splash/SplashXPath.h \
    splash/SplashXPathScanner.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../../../build-TextLab/TextLabServer/debug
} else {
    DESTDIR = $$PWD/../../../build-TextLab/TextLabServer/release
}


#INCLUDEPATH += $$PWD/poppler
INCLUDEPATH += $$PWD/../../../3dParts/include/Poppler
INCLUDEPATH += $$PWD/../../../3dParts/include/Poppler/libpng16
INCLUDEPATH += $$PWD/../../../3dParts/include/Poppler/freetype2
INCLUDEPATH += $$PWD/goo
INCLUDEPATH += $$PWD/splash
INCLUDEPATH += $$PWD/fofi
INCLUDEPATH += /usr/include/cairo
INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += /usr/include/nspr
INCLUDEPATH += /usr/include/openjpeg-2.3/
INCLUDEPATH += /usr/include/nss/


unix {
LIBS += -lz -ljpeg -ltiff -lpng -lcairo -lfontconfig -lfreetype -lnss3 -lnspr4 -lnssutil3 -lsmime3 -lssl3 -lplds4 -lplc4
}
win32: {
LIBS += -L$$DESTDIR/ -lz -ljpeg-62 -ltiff -lpng16 -lcairo -lfontconfig-1 -lfreetype -lnss3 -lnspr4 -lnssutil3 -lsmime3 -lssl-3-x64 -lplds4 -lplc4
}

QMAKE_CXXFLAGS += "-fno-sized-deallocation"

DISTFILES +=

