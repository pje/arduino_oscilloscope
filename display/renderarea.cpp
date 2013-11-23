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
  size_t num_points = this->size().width();
  this->render_points = (QPoint*) malloc(num_points * sizeof(QPoint));
  if (render_points == NULL) exit(1);
  for(size_t i = 0; i < num_points; i++) {
    render_points[i] = QPoint();
  }
  producer_thread = (pthread_t*) malloc(sizeof(pthread_t));
  if (producer_thread == NULL) exit(1);
  pthread_create(producer_thread, NULL, sample_producer_start, (void *) this->sample_backlog);
  this->redraw_timer = new QTimer(this);
  connect(redraw_timer, SIGNAL(timeout()), this, SLOT(on_redraw_timer_timeout()));
  redraw_timer->start(30);
}

RenderArea::~RenderArea() {
  redraw_timer->stop();
  delete redraw_timer;
  delete pen;
  ring_buffer_free(sample_backlog);
  pthread_detach(*producer_thread);
  free(render_points);
}

void RenderArea::on_redraw_timer_timeout() {
  for (size_t i = 0; i < (size_t)this->size().width(); i++) {
    TYPE sample = ring_buffer_get(this->sample_backlog, i);
    this->render_points[i].setX(i);
    this->render_points[i].setY(sample * this->size().height());
  }
  this->update();
}

void RenderArea::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
  painter.setPen(*(this->pen));
  painter.drawPolyline(render_points, this->size().height());
}

QSize RenderArea::minimumSizeHint(void) const {
  return QSize(400, 400);
}

QSize RenderArea::sizeHint(void) const {
  return QSize(400, 400);
}
