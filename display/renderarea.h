#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class QPen;

struct RingBuffer;

class RenderArea : public QWidget {
  Q_OBJECT

public:
  RenderArea(QWidget *parent = 0);
  ~RenderArea();
  const static size_t numSamples = 1000;
  const static size_t default_window_size = 1000;
  QPoint *points;
  RingBuffer *ring_buffer;
  QSize minimumSizeHint(void) const;
  QSize sizeHint(void) const;

public slots:
  void on_redraw_timer_timeout();

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPen *pen;
};

#endif
