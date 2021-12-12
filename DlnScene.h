#ifndef DLNSCENE_H
#define DLNSCENE_H

#include "DlnCamera.h"
#include "DlnGeometryObject.h"
#include "DlnLightObject.h"

#include <QImage>
#include <QSharedPointer>

class DlnScene
{
public:
    DlnScene();

    void initialize(const QSizeF &viewSize);
    bool render(QImage &outputImg);

private:
    DlnCamera m_camera;

    QVector<QSharedPointer<DlnGeometryObject>> m_geometryObjects;
    QVector<QSharedPointer<DlnLightObject>> m_lightObjects;
};

#endif // DLNSCENE_H
