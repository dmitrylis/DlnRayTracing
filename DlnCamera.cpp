#include "DlnCamera.h"

DlnCamera::DlnCamera()
{
}

QVector3D DlnCamera::position() const
{
    return m_position;
}

QVector3D DlnCamera::lookAt() const
{
    return m_lookAt;
}

QVector3D DlnCamera::up() const
{
    return m_up;
}

float DlnCamera::projDistance() const
{
    return m_projDistance;
}

float DlnCamera::projHSize() const
{
    return m_projHSize;
}

float DlnCamera::aspectRatio() const
{
    return m_aspectRatio;
}

QVector3D DlnCamera::u() const
{
    return m_projU;
}

QVector3D DlnCamera::v() const
{
    return m_projV;
}

QVector3D DlnCamera::projCenter() const
{
    return m_projCenter;
}

void DlnCamera::setPosition(const QVector3D &position)
{
    m_position = position;
}

void DlnCamera::setLookAt(const QVector3D &lookAt)
{
    m_lookAt = lookAt;
}

void DlnCamera::setUp(const QVector3D &up)
{
    m_up = up;
}

void DlnCamera::setProjDistance(float distance)
{
    m_projDistance = distance;
}

void DlnCamera::setProjHSize(float hSize)
{
    m_projHSize = hSize;
}

void DlnCamera::setAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
}

void DlnCamera::updateCameraGeometry()
{
    // compute vector from camera position to the LookAt position
    m_principleAxis = m_lookAt - m_position;
    m_principleAxis.normalize();

    // compute U and V vectors of projection
    m_projU = QVector3D::crossProduct(m_principleAxis, m_up);
    m_projU.normalize();

    m_projV = QVector3D::crossProduct(m_projU, m_principleAxis);
    m_projV.normalize();

    // compute center point of projection
    m_projCenter = m_position + m_projDistance * m_principleAxis;

    // modify U and V to match the size and aspect ratio
    m_projU *= m_projHSize;
    m_projV *= (m_projHSize / m_aspectRatio);
}

bool DlnCamera::generateRay(float projX, float projY, DlnRay &ray)
{
    // compute the location of the screen point in world coordinates
    const QVector3D rayTarget = m_projCenter + (m_projU * projX) + (m_projV * projY);

    // use this point to compute the ray
    ray = DlnRay(m_position, rayTarget);

    return true;
}
