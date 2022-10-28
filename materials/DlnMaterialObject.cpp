#include "DlnMaterialObject.h"

#include "primitives/DlnGeometryObject.h"
#include "lights/DlnLightObject.h"

using namespace  dln;

DlnMaterialObject::DlnMaterialObject() {
    m_maxReflectionRays = 5;
    m_reflectionRayCount = 0;
}

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

DlnColor DlnMaterialObject::computeReflectionColor(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                                                   const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                                                   const QSharedPointer<DlnGeometryObject> &currentObject,
                                                   const DlnRay &incidentRay,
                                                   const QVector3D &intersectionPoint,
                                                   const QVector3D &localNormal)
{
    // compute the reflection vector
    QVector3D d = incidentRay.direction();
    QVector3D reflectionVector = d - 2 * QVector3D::dotProduct(d, localNormal) * localNormal;

    // construct the reflection ray
    DlnRay reflectionRay(intersectionPoint, intersectionPoint + reflectionVector);

    // cast this ray into the scene and find the closes object that intersects with
    QSharedPointer<DlnGeometryObject> closestObject;
    QVector3D closestIntersectionPoint;
    QVector3D closestLocalNormal;
    DlnColor closestLocalColor;

    bool intersected = castRay(reflectionRay,
                               geometryObjects,
                               currentObject,
                               closestObject,
                               closestIntersectionPoint,
                               closestLocalNormal,
                               closestLocalColor);

    // compute illumination for closest object assuming that there was a valid intersection
    DlnColor materialColor;
    if (intersected && m_reflectionRayCount < m_maxReflectionRays)
    {
        ++m_reflectionRayCount;
        materialColor = closestObject->computeColor(geometryObjects, lightObjects, closestObject, reflectionRay, closestIntersectionPoint, closestLocalColor);
    }

    return materialColor;
}

bool DlnMaterialObject::castRay(const DlnRay &castRay,
                                const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                                const QSharedPointer<DlnGeometryObject> &currentObject,
                                QSharedPointer<DlnGeometryObject> &object,
                                QVector3D &intersectionPoint,
                                QVector3D &localNormal,
                                DlnColor &localColor)
{
    QVector3D closestIntersectionPoint;
    QVector3D closestLocalNormal;
    DlnColor closestLocalColor;

    bool intersectionFound = false;
    float minDistance = std::numeric_limits<float>::max();

    for (auto sceneObject : geometryObjects)
    {
        if (currentObject != sceneObject)
        {
            bool valid = sceneObject->testIntersection(castRay, closestIntersectionPoint, closestLocalNormal, closestLocalColor);

            if (valid)
            {
                intersectionFound = true;

                // compute the distance between the source and the intersection point
                float distance = (closestIntersectionPoint - castRay.startPoint()).length();

                if (distance < minDistance)
                {
                    minDistance = distance;

                    object = sceneObject;
                    intersectionPoint = closestIntersectionPoint;
                    localNormal = closestLocalNormal;
                    localColor = closestLocalColor;
                }
            }
        }
    }

    return intersectionFound;
}
