#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class QPen;

struct RingBuffer;

class RenderArea : public QWidget {
  Q_OBJECT

public:
  const static size_t numSamples = 1000;
  const static size_t default_window_size = 1000;
  QPoint *points;
  RingBuffer *ring_buffer;
  RenderArea(QWidget *parent = 0);
  QSize minimumSizeHint(void) const;
  QSize sizeHint(void) const;
  size_t time;

public slots:
  void on_redraw_timer_timeout();
  void on_consume_timer_timeout();

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPen *pen;
};

#endif
