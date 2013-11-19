#include <QApplication>
#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QPen>
#include <QPoint>
#include <QSize>
#include <QWidget>
#include <math.h>
#include "renderarea.h"
#include <iostream>

extern "C" {
#include "ring_buffer.h"
}

RenderArea::RenderArea(QWidget *parent) : QWidget(parent) {
  setAutoFillBackground(true);
  pen = new QPen(QColor(10,10,10), 1);

  RingBuffer *ring_buffer = ring_buffer_init(numSamples);

  for (int i = 0; i < numSamples; i++) {
    double sample = sin(i / (double)numSamples);

    ring_buffer_push(ring_buffer, sample);
  }

  QPoint *points = (QPoint*) malloc(numSamples * sizeof(QPoint));
  int displayHeightPixels = this->sizeHint().height();

  for (int i = 0; i < numSamples; i++) {
    points[i] = QPoint(i, (displayHeightPixels * ring_buffer_pop(ring_buffer)));
  }
}

QSize RenderArea::minimumSizeHint(void) const {
  return QSize(100, 100);
}

QSize RenderArea::sizeHint(void) const {
  return QSize(400, 200);
}

void RenderArea::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(*(this->pen));
  painter.drawPolyline(points, numSamples);

  // ring_buffer_free(ring_buffer);
}
