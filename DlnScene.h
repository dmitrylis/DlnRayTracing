#ifndef DLNSCENE_H
#define DLNSCENE_H

#include "DlnImage.h"
#include "DlnCamera.h"
#include "primitives/DlnGeometryObject.h"
#include "lights/DlnLightObject.h"

#include <QSharedPointer>

namespace dln
{

class DlnScene
{
public:
    DlnScene();

    void initialize(const QSizeF &viewSize);
    bool render(DlnImage &outputImg);
    bool castRay(const DlnRay &castRay,
                 QSharedPointer<DlnGeometryObject> &object,
                 QVector3D &intersectionPoint,
                 QVector3D &localNormal,
                 DlnColor &localColor);

private:
    DlnCamera m_camera;

    QVector<QSharedPointer<DlnGeometryObject>> m_geometryObjects;
    QVector<QSharedPointer<DlnLightObject>> m_lightObjects;
};

}

#endif // DLNSCENE_H
