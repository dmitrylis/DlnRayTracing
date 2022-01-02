QT += quick gui

CONFIG += c++14 qmltypes -0fast

QML_IMPORT_NAME = com.dln.rendering
QML_IMPORT_MAJOR_VERSION = 1

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        DlnColor.cpp \
        DlnCamera.cpp \
        DlnImage.cpp \
        DlnRay.cpp \
        DlnScene.cpp \
        DlnTransform.cpp \
        DlnView.cpp \
        primitives/DlnGeometryObject.cpp \
        primitives/DlnPlane.cpp \
        primitives/DlnSphere.cpp \
        lights/DlnLightObject.cpp \
        lights/DlnPointLight.cpp \
        materials/DlnMaterialObject.cpp \
        materials/DlnSimpleMaterial.cpp

HEADERS += \
        DlnColor.h \
        DlnCamera.h \
        DlnImage.h \
        DlnRay.h \
        DlnScene.h \
        DlnTransform.h \
        DlnView.h \
        primitives/DlnGeometryObject.h \
        primitives/DlnPlane.h \
        primitives/DlnSphere.h \
        lights/DlnLightObject.h \
        lights/DlnPointLight.h \
        materials/DlnMaterialObject.h \
        materials/DlnSimpleMaterial.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
