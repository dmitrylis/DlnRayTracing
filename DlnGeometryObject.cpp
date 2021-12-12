#include "DlnGeometryObject.h"

DlnGeometryObject::DlnGeometryObject()
{
}

DlnGeometryObject::~DlnGeometryObject()
{
}

void DlnGeometryObject::setTransform(const DlnTransform &transform)
{
    m_transform = transform;
}

void DlnGeometryObject::setBaseColor(const QColor &baseColor)
{
    m_baseColor = baseColor;
}
