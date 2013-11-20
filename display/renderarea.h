#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class QPen;

struct RingBuffer;

class RenderArea : public QWidget {
  Q_OBJECT

public:
  const static int numSamples = 400;
  QPoint *points;
  RingBuffer *ring_buffer;
  RenderArea(QWidget *parent = 0);
  QSize minimumSizeHint(void) const;
  QSize sizeHint(void) const;
  size_t time;

public slots:
    void on_timer_timeout();

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPen *pen;
};

#endif
