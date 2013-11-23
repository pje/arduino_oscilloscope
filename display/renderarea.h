#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class QPen;
class QTimer;
class QPoint;
struct RingBuffer;

class RenderArea : public QWidget {
  Q_OBJECT

public:
  RenderArea(QWidget *parent = 0);
  ~RenderArea();
  RingBuffer *sample_backlog;
  const static size_t samples_in_backlog = 1000;

  QSize minimumSizeHint(void) const;
  QSize sizeHint(void) const;

public slots:
  void on_redraw_timer_timeout();

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPoint *render_points;
  pthread_t *producer_thread;
  QTimer *redraw_timer;
  QPen *pen;
};

#endif
