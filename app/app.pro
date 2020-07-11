QT += core network gui widgets sql multimedia multimediawidgets

CONFIG += c++17

TARGET = Bambooster

TEMPLATE = app

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    core/settingsutil.cpp \
    global.cpp \
    qtorrent.cpp \
    core/bencodeparser.cpp \
    core/bencodevalue.cpp \
    core/torrentinfo.cpp \
    core/trackerclient.cpp \
    core/torrent.cpp \
    core/peer.cpp \
    core/piece.cpp \
    core/block.cpp \
    core/torrentmessage.cpp \
    core/torrentserver.cpp \
    core/resumeinfo.cpp \
    core/torrentmanager.cpp \
    core/torrentsettings.cpp \
    core/remote.cpp \
    core/filecontroller.cpp \
    core/localservicediscoveryclient.cpp \
    core/trafficmonitor.cpp \
    ui/dbinterface.cpp \
    ui/mainwindow.cpp \
    ui/movieswindow.cpp \
    ui/panel.cpp \
    ui/playerwindow.cpp \
    ui/playlistwindow.cpp \
    ui/torrentslist.cpp \
    ui/torrentslistitem.cpp \
    ui/addtorrentdialog.cpp \
    ui/torrentitemdelegate.cpp \
    ui/torrentinfopanel.cpp \
    ui/settingswindow.cpp \
    ui/videowidget.cpp

HEADERS += \
    core/settingsutil.h \
    qtorrent.h \
    global.h \
    core/bencodeparser.h \
    core/bencodevalue.h \
    core/torrentinfo.h \
    core/trackerclient.h \
    core/torrent.h \
    core/peer.h \
    core/piece.h \
    core/block.h \
    core/torrentmessage.h \
    core/torrentserver.h \
    core/resumeinfo.h \
    core/torrentmanager.h \
    core/torrentsettings.h \
    core/remote.h \
    core/filecontroller.h \
    core/localservicediscoveryclient.h \
    core/trafficmonitor.h \
    ui/dbinterface.h \
    ui/mainwindow.h \
    ui/movieswindow.h \
    ui/panel.h \
    ui/playerwindow.h \
    ui/playlistwindow.h \
    ui/torrentslist.h \
    ui/torrentslistitem.h \
    ui/addtorrentdialog.h \
    ui/torrentitemdelegate.h \
    ui/torrentinfopanel.h \
    ui/settingswindow.h \
    ui/videowidget.h

RESOURCES += \
    resources.qrc

win32:RC_ICONS = ../res/icons/Bambooster/b.ico
macx:ICON = ../res/icons/Bambooster/qtorrent.icns

# Version definition
include(../version.pri)

# UNIX-specific configuration
unix:!macx: include(../unixconf.pri)

DISTFILES += \
    ui/diagram1.qmodel

FORMS += \
    playlistwindow.ui

STATECHARTS +=
