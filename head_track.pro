#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T20:17:15
#
#-------------------------------------------------

QT       += core gui opengl uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = head_track
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    irsmanager.cpp \
    irsrenderer2d.cpp \
    irsrenderer3d.cpp \
    render_handler/irshandlerhe.cpp \
    render_handler/irshandlerbase.cpp \
    cglutil.cpp

HEADERS  += mainwindow.h \
    irsmanager.h \
    irsrenderer2d.h \
    irsrenderer3d.h \
    render_handler/irshandlerbase.h \
    render_handler/irshandlerhe.h \
    cglutil.h

FORMS    += mainwindow.ui \
    handlerhe.ui

INCLUDEPATH += $(RSSDK_DIR)/include
INCLUDEPATH += $(RSSDK_DIR)/sample/common/include

CONFIG(release, debug|release) {
    win32{
        !contains(QMAKE_TARGET.arch, x86_64) {
            message("x86 release build")
            LIBS += $(RSSDK_DIR)/lib/Win32/libpxc.lib
            LIBS += $(RSSDK_DIR)/lib/Win32/libpxcmd.lib
            LIBS += $(RSSDK_DIR)/sample/common/lib/win32/v120/libpxcutils.lib
            LIBS += $(RSSDK_DIR)/sample/common/lib/win32/v120/libpxcutilsmd.lib
        }else{
            message("x64 release build")
            LIBS += $(RSSDK_DIR)/lib/x64/libpxc.lib
            LIBS += $(RSSDK_DIR)/lib/x64/libpxcmd.lib
            LIBS += $(RSSDK_DIR)/sample/common/lib/x64/v120/libpxcutils.lib
            LIBS += $(RSSDK_DIR)/sample/common/lib/x64/v120/libpxcutilsmd.lib
        }
    }
}else{
    win32{
        !contains(QMAKE_TARGET.arch, x86_64) {
            message("x86 debug build")
            LIBS += $(RSSDK_DIR)/lib/Win32/libpxc_d.lib
            LIBS += $(RSSDK_DIR)/lib/Win32/libpxcmd_d.lib
            LIBS += $(RSSDK_DIR)/sample/common/lib/win32/v120/libpxcutils_d.lib
            LIBS += $(RSSDK_DIR)/sample/common/lib/win32/v120/libpxcutilsmd_d.lib
        }else{
            message("x64 debug build")
            message($(RSSDK_DIR)/lib/x64/libpxc_d.lib)
            LIBS += $(RSSDK_DIR)/lib/x64/libpxc_d.lib
            LIBS += $(RSSDK_DIR)/lib/x64/libpxcmd_d.lib
            LIBS += $(RSSDK_DIR)/sample/common/lib/x64/v120/libpxcutils_d.lib
            LIBS += $(RSSDK_DIR)/sample/common/lib/x64/v120/libpxcutilsmd_d.lib
            LIBS += "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64\GlU32.Lib"
            LIBS += "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64\OpenGL32.Lib"
        }
    }
}


QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:libcmtd
LIBS += "C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x64\AdvAPI32.Lib"

RESOURCES += \
    resources.qrc



