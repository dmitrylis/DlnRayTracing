#include "DlnGeometryObject.h"

#include "lights/DlnLightObject.h"
#include "materials/DlnMaterialObject.h"

using namespace  dln;

DlnGeometryObject::DlnGeometryObject() {}

DlnGeometryObject::~DlnGeometryObject() {}

void DlnGeometryObject::setTransform(const DlnTransform &transform)
{
    m_transform = transform;
}

void DlnGeometryObject::setBaseColor(const DlnColor &baseColor)
{
    m_baseColor = baseColor;
}

bool DlnGeometryObject::setMaterial(const QSharedPointer<DlnMaterialObject> &material)
{
    m_material = material;
    return true;
}

DlnColor DlnGeometryObject::baseColor() const
{
    return m_baseColor;
}

DlnColor DlnGeometryObject::computeColor(const QVector<QSharedPointer<DlnGeometryObject>> &geometryObjects,
                                         const QVector<QSharedPointer<DlnLightObject>> &lightObjects,
                                         const QSharedPointer<DlnGeometryObject> &currentObject,
                                         const DlnRay &castRay,
                                         const QVector3D &intersectionPoint,
                                         const QVector3D &localNormal)
{
    if (m_material)
    {
        return m_material->computeColor(geometryObjects, lightObjects, currentObject, castRay, intersectionPoint, localNormal);
    }

    return DlnMaterialObject::computeDiffuseColor(geometryObjects, lightObjects, currentObject, intersectionPoint, localNormal, m_baseColor);
}
