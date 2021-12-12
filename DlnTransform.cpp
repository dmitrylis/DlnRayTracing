#include "DlnTransform.h"

#include <QtMath>
#include <QVector4D>

using namespace dln;

DlnTransform::DlnTransform()
{
    m_fwdTransform.setToIdentity();
    m_bckTransform.setToIdentity();
}

DlnTransform::DlnTransform(const QMatrix4x4 &fwd, const QMatrix4x4 &bck)
{
    m_fwdTransform = fwd;
    m_bckTransform = bck;
}

void DlnTransform::setTransform(const QVector3D &translation, const QVector3D &rotation, const QVector3D &scale)
{
    // define a matrix for each component of the transform
    QMatrix4x4 translationMatrix;
    QMatrix4x4 rotationMatrixX;
    QMatrix4x4 rotationMatrixY;
    QMatrix4x4 rotationMatrixZ;
    QMatrix4x4 scaleMatrix;

    // set to identity
    translationMatrix.setToIdentity();
    rotationMatrixX.setToIdentity();
    rotationMatrixY.setToIdentity();
    rotationMatrixZ.setToIdentity();
    scaleMatrix.setToIdentity();

    // translation
    translationMatrix.setColumn(3, QVector4D(translation, 1.0));

    // rotation
    rotationMatrixX.setRow(1, QVector4D(0.0, qCos(rotation[0]), -qSin(rotation[0]), 0.0));
    rotationMatrixX.setRow(2, QVector4D(0.0, qSin(rotation[0]), qCos(rotation[0]), 0.0));

    rotationMatrixY.setRow(0, QVector4D(qCos(rotation[1]), 0.0, qSin(rotation[1]), 0.0));
    rotationMatrixY.setRow(2, QVector4D(-qSin(rotation[1]), 0.0, qCos(rotation[1]), 0.0));

    rotationMatrixZ.setRow(0, QVector4D(qCos(rotation[2]), -qSin(rotation[2]), 0.0, 0.0));
    rotationMatrixZ.setRow(1, QVector4D(qSin(rotation[2]), qCos(rotation[2]), 0.0, 0.0));

    // scale
    scaleMatrix.setRow(0 , QVector4D(scale[0], 0.0, 0.0, 0.0));
    scaleMatrix.setRow(1 , QVector4D(0.0, scale[1], 0.0, 0.0));
    scaleMatrix.setRow(2 , QVector4D(0.0, 0.0, scale[2], 0.0));

    // combine transformations
    m_fwdTransform = translationMatrix * scaleMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ;
    m_bckTransform = m_fwdTransform.inverted();
}

QMatrix4x4 DlnTransform::forward() const
{
    return m_fwdTransform;
}

QMatrix4x4 DlnTransform::backward() const
{
    return m_bckTransform;
}

DlnRay DlnTransform::apply(const DlnRay &ray, bool directionFlag)
{
    if (directionFlag)
    {
        return DlnRay(apply(ray.startPoint(), FWDTFORM), apply(ray.endPoint(), FWDTFORM));
    }
    else
    {
        return DlnRay(apply(ray.startPoint(), BCKTFORM), apply(ray.endPoint(), BCKTFORM));
    }
}

QVector3D DlnTransform::apply(const QVector3D &vector, bool directionFlag)
{
    QVector4D tmpVector(vector, 1.0);

    if (directionFlag)
    {
        tmpVector = m_fwdTransform * tmpVector;
    }
    else
    {
        tmpVector = m_bckTransform * tmpVector;
    }

    return QVector3D(tmpVector);
}

DlnTransform DlnTransform::operator=(const DlnTransform &rhs)
{
    if (this != &rhs)
    {
        m_fwdTransform = rhs.m_fwdTransform;
        m_bckTransform = rhs.m_bckTransform;
    }
    return *this; // warning
}

namespace dln {

DlnTransform operator*(const DlnTransform &lhs, const DlnTransform &rhs)
{
    const QMatrix4x4 fwdResult = lhs.m_fwdTransform * rhs.m_bckTransform;

    return DlnTransform(fwdResult, fwdResult.inverted());
}

}


