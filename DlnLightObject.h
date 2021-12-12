#ifndef DLNLIGHTOBJECT_H
#define DLNLIGHTOBJECT_H

#include "DlnGeometryObject.h"

#include <QColor>
#include <QVector>
#include <QVector3D>
#include <QSharedPointer>

class DlnLightObject
{
public:
    DlnLightObject();
    virtual ~DlnLightObject();

    void setColor(const QColor &color);
    void setPosition(const QVector3D &position);

    virtual bool computeIllumination(const QVector3D &intersectionPoint,
                                     const QVector3D &localNormal,
                                     const QVector<QSharedPointer<DlnGeometryObject>> &objects,
                                     const QSharedPointer<DlnGeometryObject> &currentObject,
                                     QColor &color,
                                     float &intensity) const = 0;

protected:
    QColor m_color;
    QVector3D m_position;
    float m_intensity {0.0};
};

#endif // DLNLIGHTOBJECT_H
