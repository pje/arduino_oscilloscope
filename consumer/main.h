#ifndef MAIN_H
#define MAIN_H

struct GLFWwindow;

static void error_callback(int error, const char* description);
static void initialize(void);
static void window_resize_callback(GLFWwindow* window, int width, int height);
static void draw_grid(void);
static void draw_waveform(void);
void update(void);
int main(void);

#endif
