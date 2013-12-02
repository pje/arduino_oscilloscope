#ifndef MAIN_H
#define MAIN_H

struct GLFWwindow;
struct RingBuffer;

const size_t grid_divisions_vertical = 5;
const size_t grid_dot_width = 2;
const size_t max_window_width = 2048;
const size_t default_window_width = 512;
const size_t default_window_height = default_window_width;
const char *default_window_title = "oscilloscope";
size_t current_width = default_window_width;
size_t current_height = default_window_height;
size_t sizeof_ring_buffer = max_window_width;
GLFWwindow* main_window = NULL;
RingBuffer *ring_buffer = NULL;
TYPE *samples_drawable = NULL;
pthread_mutex_t *samples_drawable_lock = NULL;
pthread_t *producer_thread = NULL;

static void error_callback(int error, const char* description);
static void initialize(void);
static void window_resize_callback(GLFWwindow* window, int width, int height);
static void draw_grid(void);
static void draw_waveform(void);
void update(void);
int main(void);

#endif
