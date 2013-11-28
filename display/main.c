#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include "ring_buffer.h"
#include "sample_producer.h"
#include "colors.h"

unsigned long long i = 0;
GLFWwindow* window;
const size_t default_width = 512;
const size_t default_height = 512;
size_t current_width = default_width;
size_t current_height = default_height;
RingBuffer *ring_buffer;
TYPE *samples_drawable;
size_t sizeof_samples_drawable = default_width;
size_t sizeof_ring_buffer = 2048;
pthread_mutex_t *samples_drawable_lock;
pthread_t *producer_thread;

static void error_callback(int error, const char* description) {
  fputs(description, stderr);
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
  current_width = width;
  current_height = height;
  sizeof_samples_drawable = current_width;
  pthread_mutex_lock(samples_drawable_lock);
  if (samples_drawable) { free(samples_drawable); }
  samples_drawable = (TYPE*) calloc(sizeof_samples_drawable, sizeof(TYPE));
  pthread_mutex_unlock(samples_drawable_lock);
  glOrtho(0, current_width, 0, current_height, 0, 1);
}

void initialize(void) {
  samples_drawable_lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
  samples_drawable = (TYPE*) calloc(sizeof_samples_drawable, sizeof(TYPE));
  ring_buffer = ring_buffer_init(sizeof_ring_buffer);
  producer_thread = malloc(sizeof(pthread_t));
  pthread_create(producer_thread, NULL, sample_producer_start, ring_buffer);
  glfwSetWindowSizeCallback(window, window_resize_callback);
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) exit(EXIT_FAILURE);
  window = glfwCreateWindow(default_width, default_height, "oscil", NULL, NULL);
  if (!window) { glfwTerminate(); exit(EXIT_FAILURE); }
  glfwMakeContextCurrent(window);
  glOrtho(0, current_width, 0, current_height, 0, 1);
  printf("initialize() finished\n");
}

void update(void) {
  pthread_mutex_lock(samples_drawable_lock);
  pthread_mutex_lock(ring_buffer->elements_lock);
  memset(samples_drawable, 0, sizeof(TYPE) * sizeof_samples_drawable);
  for(size_t i = 0; i < sizeof_samples_drawable; i++){
    TYPE sample = ring_buffer_get(ring_buffer, i);
    samples_drawable[i] = sample;
  }
  pthread_mutex_unlock(ring_buffer->elements_lock);
  pthread_mutex_unlock(samples_drawable_lock);

  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(base03[0], base03[1], base03[2], base03[3]);
  glColor4fv(yellow);
  glBegin(GL_LINE_STRIP);
  pthread_mutex_lock(samples_drawable_lock);
  for (size_t j = 0; j < sizeof_samples_drawable; j++) {
    TYPE sample = samples_drawable[sizeof_samples_drawable - j];
    int gl_x_coord = j;
    int gl_y_coord = (int) round(sample * (double)current_height);
    glVertex2i(gl_x_coord, gl_y_coord);
  }
  pthread_mutex_unlock(samples_drawable_lock);
  glEnd();

  glfwSwapBuffers(window);
  glfwPollEvents();
}

int main(void) {
  initialize();
  while (!glfwWindowShouldClose(window)) {
    update();
    usleep(10);
  }
}