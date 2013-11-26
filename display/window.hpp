#ifndef WINDOW_HPP
#define WINDOW_HPP

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
