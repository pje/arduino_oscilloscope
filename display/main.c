#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <math.h>
#include "ring_buffer.h"
#include "sample_producer.h"
#include "colors.h"

size_t i = 0;
GLFWwindow* window;
const size_t default_width = 512;
const size_t default_height = 512;
size_t current_width = default_width;
size_t current_height = default_height;
RingBuffer *ring_buffer;
TYPE *samples_drawable;
size_t sizeof_samples_drawable = default_width;
size_t sizeof_ring_buffer = 1024;

static void error_callback(int error, const char* description) {
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwHideWindow(window);
  }
}

void initialize(void) {
  ring_buffer = ring_buffer_init(sizeof_ring_buffer);
  samples_drawable = calloc(sizeof_samples_drawable, sizeof(TYPE));
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) exit(EXIT_FAILURE);
  window = glfwCreateWindow(default_width, default_height, "oscil", NULL, NULL);
  if (!window) { glfwTerminate(); exit(EXIT_FAILURE); }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glOrtho(0, current_width, 0, current_height, 0, 1);
  glfwGetWindowSize(window, (int *)&current_width, (int *)&current_height);
}

void on_resize() {
  sizeof_samples_drawable = current_width;
  if (samples_drawable != NULL) {
    free(samples_drawable);
  }
  samples_drawable = calloc(sizeof_samples_drawable, sizeof(TYPE));
}

void update(void) {
  glfwGetWindowSize(window, (int *)&current_width, (int *)&current_height);

  // if (sizeof_samples_drawable != current_width) { on_resize(); }

  TYPE sample = (((++i) % current_width) / (double) current_width);
  pthread_mutex_lock(ring_buffer->elements_lock);
  ring_buffer_push(ring_buffer, sample);
  pthread_mutex_unlock(ring_buffer->elements_lock);

  pthread_mutex_lock(ring_buffer->elements_lock);
  ring_buffer_get_n(ring_buffer, sizeof_samples_drawable, samples_drawable);
  pthread_mutex_unlock(ring_buffer->elements_lock);

  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(base03[0], base03[1], base03[2], base03[3]);
  glColor4fv(yellow);

  glBegin(GL_LINE_STRIP);
  for (size_t j = 0; j < sizeof_samples_drawable; j++) {
    TYPE sample = samples_drawable[sizeof_samples_drawable - j];
    int gl_x_coord = j;
    int gl_y_coord = (int) round(sample * (double)current_height);
    glVertex2i(gl_x_coord, gl_y_coord);
  }
  glEnd();
  glFlush();
  glfwSwapBuffers(window);
  glfwPollEvents();
}

int main(void) {
  initialize();
  while (!glfwWindowShouldClose(window)) {
    update();
  }
}
