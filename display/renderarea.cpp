#include <QApplication>
#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QPen>
#include <QPoint>
#include <QSize>
#include <QWidget>

#include "renderarea.h"
#include "ring_buffer.h"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent) {
  setAutoFillBackground(true);
  pen = new QPen(QColor(10,10,10), 1);
  samples = (double*) malloc(sizeof(double) * numSamples);
  for (int i = 0; i < numSamples; i++) {
    samples[i] = i / (double)numSamples;
  }
  // void *memcpy(void *destination, const void *source, size_t num);
  memcpy(samples + 200, samples, 200 * sizeof(double));
}

QSize RenderArea::minimumSizeHint(void) const {
  return QSize(100, 100);
}

QSize RenderArea::sizeHint(void) const {
  return QSize(400, 200);
}

void RenderArea::paintEvent(QPaintEvent *event) {
  // #!/usr/bin/env ruby
  // (0..1000).select{ |i| i % 6 == 0 }.map { |i| i / 1000.0 }.*(3).first(400)

  QPoint points[numSamples];
  int displayHeightPixels = this->sizeHint().height();

  for (int i = 0; i < numSamples; i++) {
    points[i] = QPoint(i, (displayHeightPixels * samples[i]));
  }

  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(*(this->pen));
  painter.drawPolyline(points, numSamples);
}
