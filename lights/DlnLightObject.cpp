#include "DlnLightObject.h"

using namespace dln;

DlnLightObject::DlnLightObject() {}

DlnLightObject::~DlnLightObject() {}

DlnColor DlnLightObject::color() const
{
    return m_color;
}

QVector3D DlnLightObject::position() const
{
    return m_position;
}

void DlnLightObject::setColor(const DlnColor &color)
{
    m_color = color;
}

void DlnLightObject::setPosition(const QVector3D & position)
{
    m_position = position;
}
