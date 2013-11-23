#include <QApplication>
#include "window.h"
#include <sys/resource.h>

int main(int argc, char *argv[]) {
  struct rlimit rl = { .rlim_cur = 32000, .rlim_max = 32000 };
  setrlimit(RLIMIT_AS, &rl);

  QApplication app(argc, argv);
  Window window;
  window.show();
  return app.exec();
}
