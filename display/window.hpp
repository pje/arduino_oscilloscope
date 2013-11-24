#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class RenderArea;

class Window : public QWidget {
  Q_OBJECT

public:
  Window(void);

private:
  RenderArea *renderArea;
};

#endif
