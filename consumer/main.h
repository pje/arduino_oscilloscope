#ifndef MAIN_H
#define MAIN_H

struct GLFWwindow;
struct RingBuffer;

static GLFWwindow* main_window = NULL;
static const size_t max_window_width = 2048;
static const size_t default_window_width = 512;
static const size_t default_window_height = default_window_width;
static size_t current_width = default_window_width;
static size_t current_height = default_window_height;
static RingBuffer *ring_buffer = NULL;
static TYPE *samples_drawable = NULL;
static size_t sizeof_ring_buffer = max_window_width;
static pthread_mutex_t *samples_drawable_lock = NULL;
static pthread_t *producer_thread = NULL;
static const size_t mutex_attempts = 100;
static const size_t grid_divisions_vertical = 5;
static const size_t grid_dot_width = 2;

static void error_callback(int error, const char* description);
static void initialize(void);
static void window_resize_callback(GLFWwindow* window, int width, int height);
static void draw_grid(void);
static void draw_waveform(void);
void update(void);
int main(void);

#endif
