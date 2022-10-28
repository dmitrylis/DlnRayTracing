#ifndef DLNMATERIALOBJECT_H
#define DLNMATERIALOBJECT_H

#include "DlnColor.h"
#include "DlnRay.h"

#include <QVector3D>

namespace dln
{

class DlnGeometryObject;
class DlnLightObject;

class DlnMaterialObject
{
public:
    DlnMaterialObject();
    virtual ~DlnMaterialObject();

    virtual DlnColor computeColor(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                                  const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                                  const QSharedPointer<DlnGeometryObject> &currentObject,
                                  const DlnRay &castRay,
                                  const QVector3D &intersectionPoint,
                                  const QVector3D &localNormal) = 0;

    static DlnColor computeDiffuseColor(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                                        const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                                        const QSharedPointer<DlnGeometryObject> &currentObject,
                                        const QVector3D &intersectionPoint,
                                        const QVector3D &localNormal,
                                        const DlnColor &localColor);

    DlnColor computeReflectionColor(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                                    const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                                    const QSharedPointer<DlnGeometryObject> &currentObject,
                                    const DlnRay &incidentRay,
                                    const QVector3D &intersectionPoint,
                                    const QVector3D &localNormal);

    bool castRay(const DlnRay &castRay,
                 const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                 const QSharedPointer<DlnGeometryObject> &currentObject,
                 QSharedPointer<DlnGeometryObject> &object,
                 QVector3D &intersectionPoint,
                 QVector3D &localNormal,
                 DlnColor &localColor);

//private:
    inline static int m_maxReflectionRays;
    inline static int m_reflectionRayCount;
};

}

#endif // DLNMATERIALOBJECT_H
