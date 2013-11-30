#ifndef MAIN_H
#define MAIN_H

struct GLFWwindow;
struct RingBuffer;

static GLFWwindow* window = NULL;
static const size_t max_window_width = 2048;
static const size_t default_window_width = 512;
static const size_t default_window_height = 512;
static size_t current_width = default_window_width;
static size_t current_height = default_window_height;
static size_t sizeof_ring_buffer = 2048;
static RingBuffer *ring_buffer = NULL;
static TYPE *samples_drawable = NULL;
static pthread_mutex_t *samples_drawable_lock = NULL;
static pthread_t *producer_thread = NULL;
static const size_t mutex_attempts = 100;
static const unsigned short int grid_divisions_vertical = 5;
static const unsigned short int grid_dot_width = 2;

static void error_callback(int error, const char* description);
static void initialize(void);
static void window_resize_callback(GLFWwindow* window, int width, int height);
static void draw_grid(void);
static void draw_waveform(void);
void update(void);
int main(void);

#endif
