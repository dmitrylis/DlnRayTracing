#include "DlnColor.h"

using namespace dln;

DlnColor::DlnColor() : QVector3D(0.0, 0.0, 0.0) {}

DlnColor::DlnColor(float r, float g, float b) : QVector3D(r, g, b) {}

DlnColor::DlnColor(const QVector3D &vector) : QVector3D(vector) {}

DlnColor::DlnColor(const QColor &color) : QVector3D(color.redF(), color.greenF(), color.blueF()) {}

float DlnColor::red() const
{
    return x();
}

float DlnColor::green() const
{
    return y();
}

float DlnColor::blue() const
{
    return z();
}
