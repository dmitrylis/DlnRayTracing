#ifndef DLNVIEW_H
#define DLNVIEW_H

#include "DlnScene.h"

#include <QImage>
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
    void updateFrame(const QImage &img);

protected:
    QSGNode *updatePaintNode(QSGNode *old, UpdatePaintNodeData *);

private:
    QImage m_Img;
    QMutex m_imgMutex;

    DlnScene m_scene;
};

}

#endif // DLNVIEW_H
