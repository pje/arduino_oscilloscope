#include <QtGui>
#include "renderarea.h"
#include "window.h"

const int IdRole = Qt::UserRole;

Window::Window(void) {
  renderArea = new RenderArea;
  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->setColumnStretch(0, 1);
  mainLayout->setColumnStretch(3, 1);
  mainLayout->addWidget(renderArea, 0, 0, 1, 4);
  mainLayout->setRowMinimumHeight(1, 6);
  mainLayout->setRowMinimumHeight(8, 6);
  setLayout(mainLayout);
  setWindowTitle(tr("Oscilloscope"));
  renderArea->update();
}
