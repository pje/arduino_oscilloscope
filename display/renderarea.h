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
  const static size_t samples_in_backlog = 6000;
  const static size_t samples_per_second = 1200;
  const static size_t default_window_size = 1000;

  size_t num_samples_to_draw;
  double *samples_drawable;

  QPoint *points;
  RingBuffer *sample_backlog;
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
