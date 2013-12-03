#define GLFW_INCLUDE_GLU

#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include "ring_buffer.h"
#include "signal_source.h"
#include "colors.h"
#include "main.h"
#include "thread_utils.h"

#define EPSILON 0.0000001
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static void error_callback(int error, const char* description) {
    fputs(description, stderr);
    printf("error number: %d\n", error);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) {
        return;
    }
    switch (key) {
    case GLFW_KEY_UP:
        display_zoom = MIN(display_zoom + 0.1, 3.9);
        printf("zoom: %f\n", display_zoom);
        update();
        break;
    case GLFW_KEY_DOWN:
        display_zoom = MAX(display_zoom - 0.1, 0.1);
        printf("zoom: %f\n", display_zoom);
        update();
        break;
    default:
        break;
    }
}

static void initialize(void) {
    samples_drawable_lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(samples_drawable_lock, NULL);
    pthread_mutex_unlock(samples_drawable_lock);
    samples_drawable = (TYPE*) calloc(max_window_width, sizeof(TYPE));
    ring_buffer = ring_buffer_init(sizeof_ring_buffer);
    producer_thread = malloc(sizeof(pthread_t));
    pthread_create(producer_thread, NULL, signal_source_start, ring_buffer);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    main_window = glfwCreateWindow(default_window_width, default_window_height, default_window_title, NULL, NULL);
    if (!main_window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(main_window);
    glDisable(GL_DEPTH_TEST);
    glOrtho(0, current_width, 0, current_height, 0, 1);
}

void update(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    draw_grid();
    draw_waveform();
    glfwSwapBuffers(main_window);
    glfwPollEvents();
}

static void draw_waveform(void) {
    int n = MIN((int) round(current_width * display_zoom), sizeof_ring_buffer);
    int m = current_width;

    while (pthread_mutex_trylock(samples_drawable_lock) != 0) {
        nanosecond_sleep();
    }
    while (pthread_mutex_trylock(ring_buffer->elements_lock) != 0) {
        nanosecond_sleep();
    }

    memset(samples_drawable, 0, sizeof(TYPE) * current_width);
    for(size_t i = 0; i < current_width; i++) {
        int sample_offset = (int) round((i * n + m / 2.0) / m);
        TYPE sample = ring_buffer_get(ring_buffer, sample_offset);
        samples_drawable[i] = sample;
    }
    pthread_mutex_unlock(ring_buffer->elements_lock);
    pthread_mutex_unlock(samples_drawable_lock);

    glClearColor(base3[0], base3[1], base3[2], base3[3]);
    glColor4fv(orange);
    glLineWidth(1.0);
    glBegin(GL_LINE_STRIP);

    while (pthread_mutex_trylock(samples_drawable_lock) != 0) {
        nanosecond_sleep();
    }

    for (size_t j = 0; j < current_width; j++) {
        if (j == 0) {
            continue;
        }
        TYPE sample = samples_drawable[current_width - j];
        double gl_x_coord = j;
        double gl_y_coord = sample * current_height;
        if (fabs(gl_y_coord) < EPSILON) {
            gl_y_coord = 1.0;
        }
        glVertex2f(gl_x_coord, gl_y_coord);
    }
    pthread_mutex_unlock(samples_drawable_lock);
    glEnd();
}

static void draw_grid(void) {
    glLineWidth(1.0);
    glColor4fv(base2);
    size_t vertical_division_height = (int) round(current_height / (double)grid_divisions_vertical);
    for (size_t i = 0; i < (grid_divisions_vertical - 1); i++) {
        size_t gl_y0_coord = vertical_division_height + (vertical_division_height * i);
        size_t gl_y1_coord = gl_y0_coord;
        for (size_t j = 0; j < current_width; j+=(grid_dot_width + 1)) {
            glBegin(GL_LINE_STRIP);
            size_t gl_x0_coord = j;
            size_t gl_x1_coord = j+grid_dot_width;
            glVertex2i(gl_x0_coord, gl_y0_coord);
            glVertex2i(gl_x1_coord, gl_y1_coord);
            glEnd();
        }
    }
}

static void window_resize_callback(GLFWwindow* window, int width, int height) {
    current_width = width;
    current_height = height;
    glViewport(0, 0, current_width, current_height);
    glLoadIdentity();
    glOrtho(0, current_width, 0, current_height, 0, 1);
    update();
}

int main(void) {
    initialize();
    glfwSetErrorCallback(error_callback);
    glfwSetWindowSizeCallback(main_window, window_resize_callback);
    glfwSetFramebufferSizeCallback(main_window, window_resize_callback);
    glfwSetKeyCallback(main_window, key_callback);
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(main_window)) {
        update();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
