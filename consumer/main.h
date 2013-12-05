#ifndef MAIN_H
#define MAIN_H

struct GLFWwindow;
struct RingBuffer;

const size_t grid_divisions_vertical = 5;
const size_t grid_dot_width = 3;
const size_t grid_dot_stride = 2;

GLFWwindow* main_window = NULL;
const char *default_window_title = "oscilloscope";
const unsigned int max_window_width = 2048;
const unsigned int default_window_width = 512;
const unsigned int default_window_height = default_window_width;
const double max_display_zoom = 3.9;
const double min_display_zoom = 0.1;
const double scroll_to_zoom_translation_factor = 0.03;
double display_zoom = 1.0;
unsigned int current_width = default_window_width;
unsigned int current_height = default_window_height;
unsigned int sizeof_ring_buffer = max_window_width;
RingBuffer *ring_buffer = NULL;
TYPE *samples_drawable = NULL;
pthread_mutex_t *samples_drawable_lock = NULL;
pthread_t *producer_thread = NULL;

volatile sig_atomic_t event_flag_resized = 0;
volatile sig_atomic_t new_width = 512;
volatile sig_atomic_t new_height = 512;
volatile sig_atomic_t event_flag_zoomed = 0;
volatile double new_display_zoom = 1.0;

int grid_index = 0;

static void error_callback(int error, const char* description);
static void register_debugging_signal_handlers();
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void initialize(void);
static void initialize_grid(void);
static void window_resize_callback(GLFWwindow* window, int width, int height);
static void resize(GLFWwindow* window, int width, int height);
static void set_gl_view(int width, int height);
static void draw_grid(void);
static void draw_waveform(void);
static void redraw(void);
int main(void);

#endif
