#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

class RenderArea : public QWidget {
  Q_OBJECT

public:
  RenderArea(QWidget *parent = 0);
  QSize minimumSizeHint(void) const;
  QSize sizeHint(void) const;

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPen pen;
  QBrush brush;
};

#endif
