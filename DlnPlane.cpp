#include "DlnPlane.h"

using namespace dln;

DlnPlane::DlnPlane() {}

DlnPlane::~DlnPlane() {}

bool DlnPlane::testIntersection(const DlnRay &castRay, QVector3D &intersectionPoint, QVector3D &localNormal, QColor &localColor)
{
    // copy the ray and apply backward transform
    DlnRay bckRay = m_transform.apply(castRay, dln::BCKTFORM);

    QVector3D k = bckRay.direction().normalized();

    // Check if there is an intersection (cast ray is not parallel to the plane)
    if (!qFuzzyIsNull(k.z()))
    {
        const float t = bckRay.startPoint().z() / -k.z();

        // if t is negative, then intersection point behind the camera and we can ignore it
        if (t > 0.0)
        {
            // compute values for U and V
            const float u = bckRay.startPoint().x() + (k.x() * t);
            const float v = bckRay.startPoint().y() + (k.y() * t);

            // if the magnitude of U and V is less or equal to one - we are inside the plane
            if (qAbs(u) < 1.0 && qAbs(v) < 1.0)
            {
                // compute point of intersection
                QVector3D poi = bckRay.startPoint() + t * k;

                // transform the intersection point back to world coordinates
                intersectionPoint = m_transform.apply(poi, dln::FWDTFORM);

                // compute the local normal
                QVector3D localOrigin; // zero point
                QVector3D normal(0.0, 0.0, -1.0);
                QVector3D globalOrigin = m_transform.apply(localOrigin, dln::FWDTFORM);

                localNormal = m_transform.apply(normal, dln::FWDTFORM) - globalOrigin;
                localNormal.normalize();

                localColor = m_baseColor;

                return true;
            }
        }
    }

    return false;
}
