PROJECT_ROOT=$(CURDIR)
NAME=consumer
SOURCES=arduino_serial_port.c ring_buffer.c signal_source.c
HEADERS=arduino_serial_port.h ring_buffer.h signal_source.h colors.h main.h
MAIN=main.c
TARGET=$(NAME)
CC=clang-3.3
CFLAGS= -Wall -std=c11 -g -O0

build: $(TARGET)

clean:
	rm -rf *.o
	rm -rf $(TARGET)
	rm -rf ring_buffer_test

run:
	./$(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) `pkg-config --cflags glfw3` -o consumer $(MAIN) $(SOURCES) `pkg-config --static --libs glfw3`

ring_buffer_test: ring_buffer.c ring_buffer_test.c ring_buffer.h
	$(CC) $(CFLAGS) ring_buffer.c ring_buffer_test.c -o ring_buffer_test

.PHONY: build clean run
.DEFAULT: $(TARGET)
