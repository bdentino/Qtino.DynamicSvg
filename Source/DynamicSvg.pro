TEMPLATE = lib
TARGET = DynamicSvg
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Qtino.DynamicSvg

# Input
SOURCES += \
    *.cpp

HEADERS += \
    *.h

OTHER_FILES = qmldir

QMAKE_MOC_OPTIONS += -Muri=$$uri

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

cleanTarget.files +=
cleanTarget.path += $$installPath
macx: cleanTarget.extra = rm -rf $$installPath
ios: cleanTarget.extra = rm -rf $$installPath

qmldir.files = qmldir
qmldir.path = $$installPath
target.path = $$installPath

INSTALLS += target qmldir

QT  += core-private gui-private
qtHaveModule(widgets): QT += widgets-private

DEFINES   += QT_NO_USING_NAMESPACE
win32-msvc*|win32-icc:QMAKE_LFLAGS += /BASE:0x66000000
solaris-cc*:QMAKE_CXXFLAGS_RELEASE -= -O2

HEADERS += \
    QtSvg/qsvggraphics_p.h        \
    QtSvg/qsvghandler_p.h         \
    QtSvg/qsvgnode_p.h            \
    QtSvg/qsvgstructure_p.h       \
    QtSvg/qsvgstyle_p.h           \
    QtSvg/qsvgfont_p.h            \
    QtSvg/qsvgtinydocument_p.h    \
    QtSvg/qsvgrenderer.h          \
    QtSvg/qsvgwidget.h            \
    QtSvg/qgraphicssvgitem.h      \
    QtSvg/qsvggenerator.h \
    QtSvg/qtsvgglobal.h


SOURCES += \
    QtSvg/qsvggraphics.cpp        \
    QtSvg/qsvghandler.cpp         \
    QtSvg/qsvgnode.cpp            \
    QtSvg/qsvgstructure.cpp       \
    QtSvg/qsvgstyle.cpp           \
    QtSvg/qsvgfont.cpp            \
    QtSvg/qsvgtinydocument.cpp    \
    QtSvg/qsvgrenderer.cpp        \
    QtSvg/qsvgwidget.cpp          \
    QtSvg/qgraphicssvgitem.cpp    \
    QtSvg/qsvggenerator.cpp

INCLUDEPATH += QtSvg

wince*: {
    SOURCES += \
        QtSvg/qsvgfunctions_wince.cpp
    HEADERS += \
        QtSvg/qsvgfunctions_wince_p.h
}

contains(QT_CONFIG, system-zlib) {
    if(unix|mingw):          LIBS_PRIVATE += -lz
    else:                    LIBS += zdll.lib
} else {
    git_build: \
        INCLUDEPATH += $$[QT_INSTALL_HEADERS/get]/QtZlib
    else: \
        INCLUDEPATH += $$[QT_INSTALL_HEADERS/src]/QtZlib
}

ANDROID_LIB_DEPENDENCIES = \
    lib/libQt5Xml.so

ANDROID_BUNDLED_FILES += \
    lib/libQt5Xml.so
