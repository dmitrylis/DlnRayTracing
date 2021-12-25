#ifndef DLNSCENE_H
#define DLNSCENE_H

#include "DlnImage.h"
#include "DlnCamera.h"
#include "DlnGeometryObject.h"
#include "DlnLightObject.h"

#include <QSharedPointer>

namespace dln
{

class DlnScene
{
public:
    DlnScene();

    void initialize(const QSizeF &viewSize);
    bool render(DlnImage &outputImg);

private:
    DlnCamera m_camera;

    QVector<QSharedPointer<DlnGeometryObject>> m_geometryObjects;
    QVector<QSharedPointer<DlnLightObject>> m_lightObjects;
};

}

#endif // DLNSCENE_H
