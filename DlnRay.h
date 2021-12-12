#ifndef DLNRAY_H
#define DLNRAY_H

#include <QVector3D>

class DlnRay
{
public:
    DlnRay();
    DlnRay(const QVector3D &startPoint, const QVector3D &endPoint);

    QVector3D startPoint() const;
    QVector3D endPoint() const;
    QVector3D direction() const;

private:
    QVector3D m_startPoint;
    QVector3D m_endPoint;
    QVector3D m_direction;
};

#endif // DLNRAY_H
