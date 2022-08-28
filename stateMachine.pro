QT += core gui network
QT += mqtt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += HAVE_CONFIG_H

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Tools/appconfig.cpp \
    Tools/log/log.cpp \
    Tools/myappconfig.cpp \
    handlemessage.cpp \
    ledcontroller.cpp \
    main.cpp \
    messagedevice.cpp \
    mqttreceiver.cpp \
    protocol.cpp \
    tools.cpp \
    widget.cpp \
    mysocket.cpp \
    myserver.cpp \
    qrencode-4.1.0/bitstream.c \
    qrencode-4.1.0/mask.c \
    qrencode-4.1.0/mmask.c \
    qrencode-4.1.0/mqrspec.c \
    qrencode-4.1.0/qrenc.c \
    qrencode-4.1.0/qrencode.c \
    qrencode-4.1.0/qrinput.c \
    qrencode-4.1.0/qrspec.c \
    qrencode-4.1.0/rsecc.c \
    qrencode-4.1.0/split.c \

HEADERS += \
    Tools/appconfig.h \
    Tools/log/log.h \
    Tools/myappconfig.h \
    handlemessage.h \
    ledcontroller.h \
    messagedevice.h \
    mqttreceiver.h \
    tools.h \
    widget.h \
    mysocket.h \
    protocol.h \
    myserver.h \
    qrencode-4.1.0/bitstream.h \
    qrencode-4.1.0/config.h \
    qrencode-4.1.0/mask.h \
    qrencode-4.1.0/mmask.h \
    qrencode-4.1.0/mqrspec.h \
    qrencode-4.1.0/qrencode.h \
    qrencode-4.1.0/qrencode_inner.h \
    qrencode-4.1.0/qrinput.h \
    qrencode-4.1.0/qrspec.h \
    qrencode-4.1.0/rsecc.h \
    qrencode-4.1.0/split.h \

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    Resource.qrc
