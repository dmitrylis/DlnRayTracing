QT += quick gui

CONFIG += c++14 qmltypes

QML_IMPORT_NAME = com.dln.rendering
QML_IMPORT_MAJOR_VERSION = 1

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        DlnCamera.cpp \
        DlnGeometryObject.cpp \
        DlnLightObject.cpp \
        DlnPlane.cpp \
        DlnPointLight.cpp \
        DlnRay.cpp \
        DlnScene.cpp \
        DlnSphere.cpp \
        DlnTransform.cpp \
        DlnView.cpp \
        SceneItem.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    DlnCamera.h \
    DlnGeometryObject.h \
    DlnLightObject.h \
    DlnPlane.h \
    DlnPointLight.h \
    DlnRay.h \
    DlnScene.h \
    DlnSphere.h \
    DlnTransform.h \
    DlnView.h \
    SceneItem.h
