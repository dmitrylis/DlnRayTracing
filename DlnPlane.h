#ifndef DLNPLANE_H
#define DLNPLANE_H

#include "DlnGeometryObject.h"

namespace dln
{

class DlnPlane : public DlnGeometryObject
{
public:
    DlnPlane();
    ~DlnPlane() override;

    bool testIntersection(const DlnRay &castRay, QVector3D &intersectionPoint, QVector3D &localNormal, QColor &localColor) override;
};

}

#endif // DLNPLANE_H
