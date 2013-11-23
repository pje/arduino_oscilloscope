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
#include <signal.h>
#include "renderarea.h"

extern "C" {
#include "ring_buffer.h"
#include "sample_producer.h"
}

RenderArea::RenderArea(QWidget *parent) : QWidget(parent) {
  setAutoFillBackground(true);
  pen = new QPen(QColor(20, 20, 20), 1);
  this->sample_backlog = ring_buffer_init(samples_in_backlog);
  size_t num_points = this->size().width();
  this->render_points = (QPoint*) malloc(num_points * sizeof(QPoint));
  if (render_points == NULL) {
    fprintf(stderr, "unable to malloc() QPoint array");
    exit(EXIT_FAILURE);
  }
  for(size_t i = 0; i < num_points; i++) {
    render_points[i] = QPoint();
  }
  this->samples_drawable = (TYPE*)calloc(this->size().width(), sizeof(TYPE));
  producer_thread = (pthread_t*) malloc(sizeof(pthread_t));
  if (producer_thread == NULL) {
    fprintf(stderr, "unable to malloc() pthread");
    exit(EXIT_FAILURE);
  }
  pthread_create(producer_thread, NULL, sample_producer_start, (void *) this->sample_backlog);
  this->redraw_timer = new QTimer(this);
  connect(redraw_timer, SIGNAL(timeout()), this, SLOT(on_redraw_timer_timeout()));
  redraw_timer->start(30); // 33 1/3 redraws per second
}

RenderArea::~RenderArea() {
  pthread_mutex_lock(sample_backlog->elements_lock);
  redraw_timer->stop();
  delete redraw_timer;
  delete pen;
  pthread_kill(*producer_thread, SIGTERM);
  pthread_mutex_unlock(sample_backlog->elements_lock);
  ring_buffer_free(sample_backlog);
  free(samples_drawable);
  free(render_points);
}

void RenderArea::on_redraw_timer_timeout() {
  size_t num_samples_to_render = this->size().width();
  pthread_mutex_lock(sample_backlog->elements_lock);
  ring_buffer_get_n(this->sample_backlog, num_samples_to_render, this->samples_drawable);
  pthread_mutex_unlock(sample_backlog->elements_lock);
  for (size_t i = 0; i < num_samples_to_render; i++) {
    this->render_points[i].setX(i);
    this->render_points[i].setY(this->samples_drawable[num_samples_to_render - i] * this->size().height());
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
