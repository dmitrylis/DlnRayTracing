#ifndef DLNSPHERE_H
#define DLNSPHERE_H

#include "DlnGeometryObject.h"

namespace dln
{

class DlnSphere : public DlnGeometryObject
{
public:
    DlnSphere();
    ~DlnSphere() override;

    bool testIntersection(const DlnRay &castRay, QVector3D &intersectionPoint, QVector3D &localNormal, QColor &localColor) override;
};

}

#endif // DLNSPHERE_H
