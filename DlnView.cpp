#include "DlnView.h"

#include <QQuickWindow>
#include <QSGGeometryNode>
#include <QSGSimpleTextureNode>
#include <QSGTexture>
#include <QtGlobal>
#include <QtMath>

namespace {
constexpr int WIDTH = 1024;
constexpr int HEIGHT = 768;
}

DlnView::DlnView(QQuickItem *parent) : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);

    setSize(QSizeF(::WIDTH, ::HEIGHT));
    m_Img = QImage(::WIDTH, ::HEIGHT, QImage::Format_RGB32);

    m_scene.initialize(size());

    // test code
    m_scene.render(m_Img);

//    connect(this, &QQuickItem::widthChanged, [this] {
//        m_Img = QImage(size().width(), size().height(), QImage::Format_RGB32);
//        m_scene.initialize(size());
//        m_scene.render(m_Img);
//        update();
//    });

//    connect(this, &QQuickItem::heightChanged, [this] {
//        m_Img = QImage(size().width(), size().height(), QImage::Format_RGB32);
//        m_scene.initialize(size());
//        m_scene.render(m_Img);
//        update();
//    });
}

void DlnView::updateFrame(const QImage &img)
{
    do
    {
        QMutexLocker locker(&m_imgMutex);
        m_Img = img.copy();
    }
    while (false);

    update();
}

QSGNode *DlnView::updatePaintNode(QSGNode *old, UpdatePaintNodeData *)
{
    QMutexLocker locker(&m_imgMutex);
    if (m_Img.isNull())
    {
        return old;
    }

    QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(old);
    if (node == nullptr)
    {
        node = new QSGSimpleTextureNode();
    }

    QSGTexture *t = window()->createTextureFromImage(m_Img.scaled(boundingRect().size().toSize()));

    if (t)
    {
        QSGTexture *tt = node->texture();
        if (tt)
        {
            tt->deleteLater();
        }
        node->setRect(boundingRect());
        node->setTexture(t);
    }

    return node;
}
