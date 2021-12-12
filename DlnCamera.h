#ifndef DLNCAMERA_H
#define DLNCAMERA_H

#include "DlnRay.h"

#include <QVector3D>

class DlnCamera
{
public:
    DlnCamera();

    QVector3D position() const;
    QVector3D lookAt() const;
    QVector3D up() const;

    float projDistance() const;
    float projHSize() const;
    float aspectRatio() const;
    QVector3D u() const;
    QVector3D v() const;
    QVector3D projCenter() const;

    void setPosition(const QVector3D &position);
    void setLookAt(const QVector3D &lookAt);
    void setUp(const QVector3D &up);
    void setProjDistance(float distance);
    void setProjHSize(float hSize);
    void setAspectRatio(float aspectRatio);

    void updateCameraGeometry();
    bool generateRay(float projX, float projY, DlnRay &ray);

private:
    QVector3D m_position {0.0, -10.0, 0.0};
    QVector3D m_lookAt {0.0, 0.0, 0.0};
    QVector3D m_up {0.0, 0.0, 1.0};
    QVector3D m_principleAxis;

    float m_projDistance {1.0};
    float m_projHSize {1.0};
    float m_aspectRatio {1.0};
    QVector3D m_projU;
    QVector3D m_projV;
    QVector3D m_projCenter;
};

#endif // DLNCAMERA_H
