#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class QPen;

class RenderArea : public QWidget {
  Q_OBJECT

public:
  const static int numSamples = 400;
  QPoint *points;
  double *samples;
  RenderArea(QWidget *parent = 0);
  QSize minimumSizeHint(void) const;
  QSize sizeHint(void) const;

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPen *pen;
};

#endif
