#ifndef DLNGEOMETRYOBJECT_H
#define DLNGEOMETRYOBJECT_H

#include "DlnRay.h"
#include "DlnTransform.h"

#include <QVector3D>
#include <QColor>

class DlnGeometryObject
{
public:
    DlnGeometryObject();
    virtual ~DlnGeometryObject();

    virtual bool testIntersection(const DlnRay &castRay,
                                  QVector3D &intersectionPoint,
                                  QVector3D &localNormal,
                                  QColor &localColor) = 0;

    void setTransform(const DlnTransform &transform);
    void setBaseColor(const QColor &baseColor);

protected:
    QColor m_baseColor {Qt::green};
    DlnTransform m_transform;
};

#endif // DLNGEOMETRYOBJECT_H
