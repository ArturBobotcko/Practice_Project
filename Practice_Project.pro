QT       += core gui multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    StyleHelper.cpp \
    add_playlist.cpp \
    databasehandler.cpp \
    main.cpp \
    mainwindow.cpp \
    musicplayer.cpp \
    playlistwindow.cpp \
    selectplaylist.cpp

HEADERS += \
    StyleHelper.h \
    add_playlist.h \
    databasehandler.h \
    mainwindow.h \
    musicplayer.h \
    playlistwindow.h \
    selectplaylist.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    add_playlist.ui \
    mainwindow.ui \
    playlist.ui \
    playlistwindow.ui \
    selectplaylist.ui

RESOURCES += \
    MainWindowResources.qrc

DISTFILES +=
