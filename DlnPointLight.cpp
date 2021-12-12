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

    // starting point
    const QVector3D startPoint = intersectionPoint; // TODO: will be updated

    // compute the angle between the local normal and light ray
    const float angle = qAcos(QVector3D::dotProduct(localNormal, toLight));

    // if the normal is pointing away from the light - there is no illumination
    if (angle > 1.5708)
    {
        // no illumination at all
        color = m_color;
        intensity = 0.0;
        return false;
    }
    else
    {
        // there is an illumination, let's compute it
        color = m_color;
        intensity = m_intensity * (1.0 - (angle / 1.5708));
        return true;
    }
}
