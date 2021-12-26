#ifndef DLNPOINTLIGHT_H
#define DLNPOINTLIGHT_H

#include "DlnLightObject.h"

namespace dln
{

class DlnPointLight : public DlnLightObject
{
public:
    DlnPointLight();
    ~DlnPointLight() override;

    bool computeIllumination(const QVector3D &intersectionPoint,
                             const QVector3D &localNormal,
                             const QVector<QSharedPointer<DlnGeometryObject>> &objects,
                             const QSharedPointer<DlnGeometryObject> &currentObject,
                             QColor &color,
                             float &intensity) const override;
};

}

#endif // DLNPOINTLIGHT_H
