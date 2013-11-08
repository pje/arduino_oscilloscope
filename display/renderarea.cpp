#include <QtGui>
#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent) {
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  pen = QPen(Qt::blue, 2);
}

QSize RenderArea::minimumSizeHint() const {
  return QSize(100, 100);
}

QSize RenderArea::sizeHint() const {
  return QSize(400, 200);
}

void RenderArea::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  static const QPoint points[4] = {
    QPoint(10, 10),
    QPoint(20, 20),
    QPoint(25, 25),
    QPoint(15, 15)
  };
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(pen);
  painter.drawPolyline(points, 4);
}
