#ifndef DLNVIEW_H
#define DLNVIEW_H

#include "DlnImage.h"
#include "DlnScene.h"

#include <QMutex>
#include <QQuickItem>
#include <QtQml/qqml.h>

namespace dln {

class DlnView : public QQuickItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DlnView)

public:
    explicit DlnView(QQuickItem *parent = nullptr);

public slots:
    void updateFrame(const DlnImage &image);

protected:
    QSGNode *updatePaintNode(QSGNode *old, UpdatePaintNodeData *);

private:
    DlnImage m_image;
    QMutex m_imageMutex;

    DlnScene m_scene;
};

}

#endif // DLNVIEW_H
