#include "DlnRay.h"

using namespace dln;

DlnRay::DlnRay() {}

DlnRay::DlnRay(const QVector3D &startPoint, const QVector3D &endPoint)
    : m_startPoint(startPoint)
    , m_endPoint(endPoint)
{
    m_direction = endPoint - startPoint;
}

QVector3D DlnRay::startPoint() const
{
    return m_startPoint;
}

QVector3D DlnRay::endPoint() const
{
    return m_endPoint;
}

QVector3D DlnRay::direction() const
{
    return m_direction;
}
