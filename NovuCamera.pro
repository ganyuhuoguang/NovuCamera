#-------------------------------------------------
#
# Project created by QtCreator 2019-12-06T18:55:08
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT       += network

TARGET = NovuCamera
TEMPLATE = app

VLC_PATH = ../packages/libvlc-2.2.2
INC_PATH = ../packages/inc
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    RenderWidget.cpp \
    served/mux/regex_matcher.cpp \
    served/mux/static_matcher.cpp \
    served/mux/variable_matcher.cpp \
    served/net/connection.cpp \
    served/net/connection_manager.cpp \
    served/net/server.cpp \
    served/plugins/access_log.cpp \
    served/methods_handler.cpp \
    served/multiplexer.cpp \
    served/parameters.cpp \
    served/request.cpp \
    served/request_parser.cpp \
    served/request_parser_impl.cpp \
    served/response.cpp \
    served/status.cpp \
    served/uri.cpp \
    mythread.cpp \
    mysql.cpp

HEADERS  += mainwindow.h \
    Util.h \
    server.h \
    RenderWidget.h \
    served/mux/empty_matcher.hpp \
    served/mux/matchers.hpp \
    served/mux/regex_matcher.hpp \
    served/mux/segment_matcher.hpp \
    served/mux/static_matcher.hpp \
    served/mux/variable_matcher.hpp \
    served/net/connection.hpp \
    served/net/connection_manager.hpp \
    served/net/server.hpp \
    served/methods.hpp \
    served/methods_handler.hpp \
    served/multiplexer.hpp \
    served/parameters.hpp \
    served/plugins.hpp \
    served/request.hpp \
    served/request_error.hpp \
    served/request_parser.hpp \
    served/request_parser_impl.hpp \
    served/response.hpp \
    served/served.hpp \
    served/status.hpp \
    served/uri.hpp \
    mythread.h \
    mysql.h \
    packages/inc/amcomdef.h \
    packages/inc/arcsoft_face_sdk.h \
    packages/inc/asvloffscreen.h \
    packages/inc/merror.h \
    person02.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv \
               /usr/local/include/opencv2 \
               $${VLC_PATH}/include \
               $${INC_PATH}  \
               /ffmpeg/include \
               /usr/local/include/oss_c_sdk \
               /usr/local/aprutil/include/apr-1 \
               /usr/local/apr/include/apr-1  \
               /usr/local/mxml/include

CONFIG(release, debug|release):
{
    LIBS += /usr/local/lib/libopencv_imgproc.so \
    /usr/local/lib/libopencv_highgui.so \
    /usr/local/lib/libopencv_core.so \
    /usr/local/lib/libopencv_imgcodecs.so \
    /usr/local/lib/libopencv_video.so \
    /usr/local/lib/libopencv_videoio.so \
    ../lib/linux_x64/libarcsoft_face_engine.so  \
    ../lib/linux_x64/libarcsoft_face.so  \
    -lboost_system -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswscale -L./ffmpeg/so  \
    -L../lib/linux_x64 -L/usr/local/lib -L/usr/local/apr/lib -L/usr/local/aprutil/lib -L/usr/local/mxml/lib  -larcsoft_face_engine -lmysqlclient -lpthread -loss_c_sdk -lcurl -lmxml -lapr-1 -laprutil-1 \
    ../lib/libserved.so \
    ../lib/libserved.so.1.4 \
    ../imageformats/libqjpeg.so \
    #../lib/libQt5Network.so.5 # temp  -larcsoft_face



#     /usr/libx86_64-linux-gnu/libarcsoft_face_engine.so  -Wl -rpath=../lib/linux_x64
#    ../lib/libarcsoft_face_engine.so
    LIBS += -L/usr/lib/ -lvlc
    LIBS += /usr/local/lib/liboss_c_sdk.so.3.0.0
    LIBS += -Wl,-rpath=./lib/linux_x64
#    LIBS += /usr/local/aprutil/lib/libaprutil-1.so
#    LIBS += /usr/local/apr/lib/libapr-1.so
#    LIBS += /usr/local/mxml/lib/libmxml.so

}
