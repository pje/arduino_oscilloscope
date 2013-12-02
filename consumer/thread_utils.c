#include <time.h>
#include <unistd.h>
#include "thread_utils.h"

void nanosecond_sleep(void) {
  const struct timespec nano_struct = { 0, 1 };
  nanosleep(&nano_struct, NULL);
}
