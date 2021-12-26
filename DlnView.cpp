#include "DlnView.h"

#include <QQuickWindow>
#include <QSGGeometryNode>
#include <QSGSimpleTextureNode>
#include <QSGTexture>
#include <QtGlobal>
#include <QtMath>

namespace {
//constexpr int WIDTH = 1024;
//constexpr int HEIGHT = 768;

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

//constexpr int WIDTH = 400;
//constexpr int HEIGHT = 300;

//constexpr int WIDTH = 200;
//constexpr int HEIGHT = 150;
}

using namespace dln;

DlnView::DlnView(QQuickItem *parent) : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);

    setSize(QSizeF(::WIDTH, ::HEIGHT));
    m_image = DlnImage(size().toSize(), DlnImage::Format_RGB32);

    m_scene.initialize(size());

    // test code
    m_scene.render(m_image);
    m_image.display();

    //connect(this, &QQuickItem::widthChanged, [this] {
    //    m_image = DlnImage(size().toSize(), DlnImage::Format_RGB32);
    //    m_scene.initialize(size());
    //    m_scene.render(m_image);
    //    m_image.display();
    //    update();
    //});
    //
    //connect(this, &QQuickItem::heightChanged, [this] {
    //    m_image = DlnImage(size().toSize(), DlnImage::Format_RGB32);
    //    m_scene.initialize(size());
    //    m_scene.render(m_image);
    //    m_image.display();
    //    update();
    //});
}

void DlnView::updateFrame(const DlnImage &image)
{
    do
    {
        QMutexLocker locker(&m_imageMutex);
        //m_image = image.copy();
    }
    while (false);

    update();
}

QSGNode *DlnView::updatePaintNode(QSGNode *old, UpdatePaintNodeData *)
{
    QMutexLocker locker(&m_imageMutex);
    if (m_image.isNull())
    {
        return old;
    }

    QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(old);
    if (node == nullptr)
    {
        node = new QSGSimpleTextureNode();
    }

    QSGTexture *t = window()->createTextureFromImage(m_image.scaled(boundingRect().size().toSize()));

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
