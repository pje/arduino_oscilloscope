#include <QApplication>
#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QPen>
#include <QPoint>
#include <QSize>
#include <QTimer>
#include <QWidget>
#include <math.h>
#include <pthread.h>
#include "renderarea.h"

extern "C" {
#include "ring_buffer.h"
#include "sample_producer.h"
}

RenderArea::RenderArea(QWidget *parent) : QWidget(parent) {
  setAutoFillBackground(true);
  pen = new QPen(QColor(10,10,10), 1);
  this->ring_buffer = ring_buffer_init(numSamples);
  this->points = (QPoint*) malloc(numSamples * sizeof(QPoint));

  pthread_t *producer_thread = (pthread_t*) malloc(sizeof(pthread_t));
  pthread_create(producer_thread, NULL, sample_producer_start, (void *) this->ring_buffer);

  QTimer *redraw_timer = new QTimer(this);
  connect(redraw_timer, SIGNAL(timeout()), this, SLOT(on_redraw_timer_timeout()));
  redraw_timer->start(10);
}

RenderArea::~RenderArea() {
  ring_buffer_free(ring_buffer);
  free(points);
}

QSize RenderArea::minimumSizeHint(void) const {
  return QSize(100, 100);
}

QSize RenderArea::sizeHint(void) const {
  return QSize(this->default_window_size, 400);
}

void RenderArea::on_redraw_timer_timeout() {
  for (size_t i = 0; i < numSamples; i++) {
    this->points[i] = QPoint(i, (this->sizeHint().height() * ring_buffer_get(ring_buffer, ((ring_buffer->size - i) % ring_buffer->size))));
  }
  this->update();
}

void RenderArea::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(*(this->pen));
  painter.drawPolyline(points, numSamples);
}
