#ifndef DLNSIMPLEMATERIAL_H
#define DLNSIMPLEMATERIAL_H

#include "DlnMaterialObject.h"

namespace dln
{

class DlnSimpleMaterial : public DlnMaterialObject
{
public:
    DlnSimpleMaterial();
    ~DlnSimpleMaterial() override;

    void setColor(const DlnColor &color);
    void setReflictivity(float reflictivity);
    void setShininess(float shininess);

    // private
    DlnColor computeColor(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                          const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                          const QSharedPointer<DlnGeometryObject> &currentObject,
                          const DlnRay &castRay,
                          const QVector3D &intersectionPoint,
                          const QVector3D &localNormal) override;

    DlnColor computeSpecular(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                             const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                             const DlnRay &castRay,
                             const QVector3D &intersectionPoint,
                             const QVector3D &localNormal);

private:
    DlnColor m_baseColor {Qt::red};
    float m_reflictivity {0.0};
    float m_shininess {0.0};
};

}

#endif // DLNSIMPLEMATERIAL_H
