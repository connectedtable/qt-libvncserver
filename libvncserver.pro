TEMPLATE = lib
CONFIG += plugin

LIBS += -lvncserver

TARGET = vncserverscreen
target.path = $$[QT_INSTALL_PLUGINS]/gfxdrivers
INSTALLS += target

HEADERS = libvncserverscreen_p.h \
          libvncserverscreen_qws.h

SOURCES = libvncserverscreen_qws.cpp \
          main.cpp
