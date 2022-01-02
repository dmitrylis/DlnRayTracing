#include "DlnMaterialObject.h"

#include "primitives/DlnGeometryObject.h"
#include "lights/DlnLightObject.h"

using namespace  dln;

DlnMaterialObject::DlnMaterialObject() {}

DlnMaterialObject::~DlnMaterialObject() {}

DlnColor DlnMaterialObject::computeDiffuseColor(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                                                const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                                                const QSharedPointer<DlnGeometryObject> &currentObject,
                                                const QVector3D &intersectionPoint,
                                                const QVector3D &localNormal,
                                                const DlnColor &localColor)
{
    // compute the intensity of illumination
    float intensity = 0.0;
    DlnColor color;
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;
    bool illuminated = false;
    bool illuminationFound = false;

    for (auto currentLight : lightObjects)
    {
        illuminated = currentLight->computeIllumination(intersectionPoint,
                                                        localNormal,
                                                        geometryObjects,
                                                        currentObject,
                                                        color,
                                                        intensity);

        if (illuminated)
        {
            illuminationFound = true;

            red += color.red() * intensity;
            green += color.green() * intensity;
            blue += color.blue() * intensity;
        }
    }

    if (illuminationFound)
    {
        red *= localColor.red();
        green *= localColor.green();
        blue *= localColor.blue();
    }

    return DlnColor(red, green, blue);
}

bool DlnMaterialObject::castRay(const DlnRay &castRay,
                                const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                                const QSharedPointer<DlnGeometryObject> &currentObject,
                                const QSharedPointer<DlnGeometryObject> &object,
                                QVector3D &intersectionPoint,
                                QVector3D &localNormal,
                                DlnColor &localColor)
{
    return true;
}
