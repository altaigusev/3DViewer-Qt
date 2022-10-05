QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 warn_off

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controller/controller.cpp \
    Model/loaderobj.cpp \
    Model/matrixAdapter.cpp \
    Model/s21_matrix_oop.cpp \
    Model/affineTransformations.cpp \
    View/eye.cpp \
    View/gifworker.cpp \
    View/memento.cpp \
    View/simpleobject3d.cpp \
    View/widget3d.cpp \
    giflib/3rdParty/giflib/dgif_lib.c \
    giflib/3rdParty/giflib/egif_lib.c \
    giflib/3rdParty/giflib/gif_err.c \
    giflib/3rdParty/giflib/gif_font.c \
    giflib/3rdParty/giflib/gif_hash.c \
    giflib/3rdParty/giflib/gifalloc.c \
    giflib/3rdParty/giflib/quantize.c \
    giflib/gifimage/doc/snippets/doc_src_qtgifimage.cpp \
    giflib/gifimage/qgifimage.cpp \
    main.cpp \
    View/mainwindow.cpp

HEADERS += \
    Controller/controller.h \
    Model/loaderobj.h \
    Model/matrixAdapter.h \
    Model/affineTransformations.h \
    Model/s21_matrix_oop.h \
    View/eye.h \
    View/gifworker.h \
    View/mainwindow.h \
    View/memento.h \
    View/simpleobject3d.h \
    View/widget3d.h \
    giflib/3rdParty/giflib/gif_hash.h \
    giflib/3rdParty/giflib/gif_lib.h \
    giflib/3rdParty/giflib/gif_lib_private.h \
    giflib/gifimage/qgifglobal.h \
    giflib/gifimage/qgifimage.h \
    giflib/gifimage/qgifimage_p.h

FORMS += \
    View/mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    3dviewer.qrc \
    shaders.qrc \

ICON = Icon/s21.icns

DISTFILES += \
    View/shaders/fshader.fsh \
    giflib/3rdParty/giflib.pri \
    giflib/3rdParty/giflib/AUTHORS \
    giflib/3rdParty/giflib/COPYING \
    giflib/3rdParty/giflib/README \
    giflib/gifimage/doc/qtgifimage.qdocconf \
    giflib/gifimage/doc/src/examples.qdoc \
    giflib/gifimage/doc/src/index.qdoc \
    giflib/gifimage/doc/src/qtgifimage.qdoc \
    giflib/gifimage/doc/src/usage.qdoc \
    giflib/gifimage/qtgifimage.pri \
    View/shaders/vshader.vsh \

SUBDIRS += \
    giflib/gifimage/doc/snippets/doc_src_qtgifimage.pro \
    giflib/gifimage/gifimage.pro \
    giflib/src.pro
