#ifndef DLNTRANSFORM_H
#define DLNTRANSFORM_H

#include "DlnRay.h"

#include <QVector3D>
#include <QMatrix4x4>

constexpr bool FWDTFORM = true;
constexpr bool BCKTFORM = false;

namespace dln
{

class DlnTransform
{
public:
    DlnTransform();
    DlnTransform(const QMatrix4x4 &fwd, const QMatrix4x4 &bck);

    void setTransform(const QVector3D &translation,
                      const QVector3D &rotation,
                      const QVector3D &scale);

    QMatrix4x4 forward() const;
    QMatrix4x4 backward() const;

    DlnRay apply(const DlnRay &ray, bool directionFlag);
    QVector3D apply(const QVector3D &vector, bool directionFlag);

    DlnTransform operator= (const DlnTransform &rhs);

    friend DlnTransform operator* (const DlnTransform &lhs, const DlnTransform &rhs);

private:
    QMatrix4x4 m_fwdTransform;
    QMatrix4x4 m_bckTransform;
};

}

#endif // DLNTRANSFORM_H
