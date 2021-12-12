#include "DlnPointLight.h"

#include <QtMath>

using namespace dln;

DlnPointLight::DlnPointLight()
{
    m_color = Qt::white;
    m_intensity = 1.0;
}

DlnPointLight::~DlnPointLight() {}

bool DlnPointLight::computeIllumination(const QVector3D &intersectionPoint, const QVector3D &localNormal, const QVector<QSharedPointer<DlnGeometryObject>> &objects, const QSharedPointer<DlnGeometryObject> &currentObject, QColor &color, float &intensity) const
{
    // vector from the intersection point to the light
    const QVector3D toLight = (m_position - intersectionPoint).normalized();

    // construct a ray from the point of intersection to the light
    DlnRay lightRay (intersectionPoint, intersectionPoint + toLight);

    // check the intersection with all of the objects except of current one
    QVector3D poi;
    QVector3D poiNormal;
    QColor poiColor;
    bool validPoint = false;

    for (auto sceneObject : objects)
    {
        if (sceneObject != currentObject)
        {
            validPoint = sceneObject->testIntersection(lightRay, poi, poiNormal, poiColor);
        }

        // If we have an intersection then no need to check further, otherwise this object is blocking light source
        if (validPoint)
        {
            break;
        }
    }

    // continue computing illumination if the light ray din't intersect with any object
    if (!validPoint)
    {
        // compute the angle between the local normal and light ray
        const float angle = qAcos(QVector3D::dotProduct(localNormal, toLight));

        // if the normal is pointing away from the light - there is no illumination
        if (angle <= 1.5708)
        {
            // there is an illumination, let's compute it
            color = m_color;
            intensity = m_intensity * (1.0 - (angle / 1.5708));
            return true;
        }
    }

    color = m_color;
    intensity = 0.0;
    return false;
}
