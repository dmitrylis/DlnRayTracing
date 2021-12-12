#include "DlnLightObject.h"

DlnLightObject::DlnLightObject()
{

}

DlnLightObject::~DlnLightObject()
{

}

void DlnLightObject::setColor(const QColor &color)
{
    m_color = color;
}

void DlnLightObject::setPosition(const QVector3D & position)
{
    m_position = position;
}
