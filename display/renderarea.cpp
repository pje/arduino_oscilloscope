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
  this->sample_backlog = ring_buffer_init(samples_in_backlog);
  this->points = (QPoint*) malloc(this->default_window_size * sizeof(QPoint));
  if (this->points == NULL) exit(1);

  this->num_samples_to_draw = this->default_window_size;
  this->samples_drawable = (double *)malloc(num_samples_to_draw * sizeof(TYPE));
  if (this->samples_drawable == NULL) exit(1);

  pthread_t *producer_thread = (pthread_t*) malloc(sizeof(pthread_t));
  if (producer_thread == NULL) exit(1);

  pthread_create(producer_thread, NULL, sample_producer_start, (void *) this->sample_backlog);

  QTimer *redraw_timer = new QTimer(this);
  connect(redraw_timer, SIGNAL(timeout()), this, SLOT(on_redraw_timer_timeout()));
  redraw_timer->start(1000);
}

RenderArea::~RenderArea() {
  ring_buffer_free(sample_backlog);
  free(samples_drawable);
  free(points);
}

QSize RenderArea::minimumSizeHint(void) const {
  return QSize(100, 100);
}

QSize RenderArea::sizeHint(void) const {
  return QSize(this->default_window_size, 400);
}

void RenderArea::on_redraw_timer_timeout() {
  ring_buffer_get_n(this->sample_backlog, this->num_samples_to_draw, this->samples_drawable);
  for (size_t i = 0; i < this->num_samples_to_draw; i++) {
    this->points[i] = QPoint(i, (this->samples_drawable[i] * this->sizeHint().height()));
  }
  this->update();
}

void RenderArea::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(*(this->pen));
  painter.drawPolyline(points, num_samples_to_draw);
}
