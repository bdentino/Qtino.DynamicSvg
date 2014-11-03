TEMPLATE = app

QT += qml quick svg

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

ANDROID_EXTRA_LIBS += $$[QT_INSTALL_QML]/Qtino/DynamicSvg/libDynamicSvg.so
