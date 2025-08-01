QT += qml quick widgets 3dcore 3drender 3dinput charts 3dquick concurrent network

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        networkmanager.cpp \
        pythonhandler.cpp

RESOURCES += qml.qrc \
    obj.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    networkmanager.h \
    pythonhandler.h

DISTFILES +=

INCLUDEPATH += D:/Afoot/python311/include
LIBS += -L D:/Afoot/python311/libs -lpython311
