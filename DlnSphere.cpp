#include "DlnSphere.h"

#include <QtMath>

using namespace dln;

DlnSphere::DlnSphere() {}

DlnSphere::~DlnSphere() {}

bool DlnSphere::testIntersection(const DlnRay &castRay, QVector3D &intersectionPoint, QVector3D &localNormal, QColor &localColor)
{
    // copy the ray and apply BACKWARDS transform
    DlnRay bckRay = m_transform.apply(castRay, BCKTFORM);

    // compute "a", "b" and "c" values
    QVector3D vhat = bckRay.direction().normalized();

    // "a" always 1.0
    const float b = 2.0 * QVector3D::dotProduct(bckRay.startPoint(), vhat);
    const float c = QVector3D::dotProduct(bckRay.startPoint(), bckRay.startPoint()) - 1.0;

    const float intTest = b * b - 4.0 * c;

    QVector3D poi;
    if (intTest > 0.0)
    {
        // compute intersection point
        const float numSqrt = qSqrt(intTest);
        double t1 = (-b + numSqrt) * 0.5;
        double t2 = (-b - numSqrt) * 0.5;

        if (t1 < 0.0 || t2 < 0.0)
        {
            return false;
        }

        if (t1 < t2)
        {
            poi = bckRay.startPoint() + (vhat * t1);
        }
        else
        {
            poi = bckRay.startPoint() + (vhat * t2);
        }

        // transform the intersection point back into world coordinates
        intersectionPoint = m_transform.apply(poi, FWDTFORM);

        // compute the local normal (for sphere it's easy)
        QVector3D origin(0.0, 0.0, 0.0);
        QVector3D newOrigin = m_transform.apply(origin, FWDTFORM);

        localNormal = intersectionPoint - newOrigin;
        localNormal.normalize();

        // compute the color
        localColor = m_baseColor;

        return true;
    }
    else
    {
        return false;
    }
}
