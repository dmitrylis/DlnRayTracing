// TODO: OBSOLETE STUFF, REMOVE IN THE FUTURE

#ifndef SCENEITEM_H
#define SCENEITEM_H

#include <QImage>
#include <QMutex>
#include <QQuickItem>
#include <QtQml/qqml.h>

class SceneItem : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

public:
  explicit SceneItem(QQuickItem *parent = nullptr);

public slots:
  void updateFrame(const QImage &img);

protected:
  QSGNode *updatePaintNode(QSGNode *old, UpdatePaintNodeData *);

private:
  QImage m_Img;
  QMutex m_imgMutex;
};

#endif // SCENEITEM_H
