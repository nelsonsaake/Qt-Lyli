QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    countupanimation.cpp \
    fileinfo.cpp \
    filerenamer.cpp \
    folderinfo.cpp \
    folderscanner.cpp \
    main.cpp \
    mainwindow.cpp \
    ticker.cpp \
    worker.cpp \
    writinganimation.cpp

HEADERS += \
    countupanimation.h \
    fileinfo.h \
    filerenamer.h \
    folderinfo.h \
    folderscanner.h \
    mainwindow.h \
    ticker.h \
    worker.h \
    writinganimation.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
