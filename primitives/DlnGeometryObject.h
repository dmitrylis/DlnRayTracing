#ifndef DLNGEOMETRYOBJECT_H
#define DLNGEOMETRYOBJECT_H

#include "DlnColor.h"
#include "DlnRay.h"
#include "DlnTransform.h"

#include <QVector3D>

namespace dln
{

class DlnLightObject;
class DlnMaterialObject;

class DlnGeometryObject
{
public:
    DlnGeometryObject();
    virtual ~DlnGeometryObject();

    virtual bool testIntersection(const DlnRay &castRay,
                                  QVector3D &intersectionPoint,
                                  QVector3D &localNormal,
                                  DlnColor &localColor) = 0;

    void setTransform(const DlnTransform &transform);
    void setBaseColor(const DlnColor &baseColor);
    bool setMaterial(const QSharedPointer<DlnMaterialObject> &material);

    DlnColor baseColor() const;

    DlnColor computeColor(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                          const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                          const QSharedPointer<DlnGeometryObject> &currentObject,
                          const DlnRay &castRay,
                          const QVector3D &intersectionPoint,
                          const QVector3D &localNormal);

protected:
    DlnColor m_baseColor {Qt::green};
    DlnTransform m_transform;
    QSharedPointer<DlnMaterialObject> m_material;
};

}

#endif // DLNGEOMETRYOBJECT_H
