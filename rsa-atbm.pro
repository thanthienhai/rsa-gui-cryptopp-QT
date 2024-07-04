QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogt.cpp \
    kyvanban.cpp \
    main.cpp \
    mainwindow.cpp \
    xacnhanvanban.cpp

HEADERS += \
    dialogt.h \
    kyvanban.h \
    mainwindow.h \
    xacnhanvanban.h

FORMS += \
    dialogt.ui \
    kyvanban.ui \
    mainwindow.ui \
    xacnhanvanban.ui

# Default rules for deployment.S
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += /usr/include/cryptopp
LIBS += -lcryptopp

INCLUDEPATH += /usr/include/pugixml.hpp

LIBS += -lzip


#INCLUDEPATH += /home/Downloads/DuckX/include

#LIBS += -L/home/Downloads/DuckX/build/ libduckx.a

#CONFIG += c++11

DISTFILES += \
    chuKy.txt \
    hamBam.txt \
    privateKey.txt \
    publicKey.txt


INCLUDEPATH += thirdparty/tinyxml2
