#ifndef DLNLIGHTOBJECT_H
#define DLNLIGHTOBJECT_H

#include "DlnColor.h"

#include <QVector>
#include <QVector3D>
#include <QSharedPointer>

namespace dln
{

class DlnGeometryObject;

class DlnLightObject
{
public:
    DlnLightObject();
    virtual ~DlnLightObject();

    DlnColor color() const;
    QVector3D position() const;

    void setColor(const DlnColor &color);
    void setPosition(const QVector3D &position);

    virtual bool computeIllumination(const QVector3D &intersectionPoint,
                                     const QVector3D &localNormal,
                                     const QVector<QSharedPointer<DlnGeometryObject>> &objects,
                                     const QSharedPointer<DlnGeometryObject> &currentObject,
                                     DlnColor &color,
                                     float &intensity) const = 0;

protected:
    DlnColor m_color;
    QVector3D m_position;
    float m_intensity {0.0};
};

}

#endif // DLNLIGHTOBJECT_H
