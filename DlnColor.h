#ifndef DLNCOLOR_H
#define DLNCOLOR_H

#include <QVector3D>
#include <QColor>

namespace dln
{

class DlnColor : public QVector3D
{
public:
    DlnColor();
    DlnColor(float r, float g, float b);
    DlnColor(const QVector3D &vector);
    DlnColor(const QColor &color);

    float red() const;
    float green() const;
    float blue() const;
};

}

#endif // DLNCOLOR_H
