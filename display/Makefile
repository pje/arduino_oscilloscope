PROJECT_ROOT=$(CURDIR)
NAME=display
SOURCES=arduino_serial_port.c ring_buffer.c sample_producer.c
HEADERS=arduino_serial_port.h ring_buffer.h sample_producer.h colors.h
MAIN=main.c
TARGET=$(NAME)
CC=clang

clean:
	rm -rf *.o
	rm -rf $(TARGET)
	rm -rf ring_buffer_test

run:
	./$(TARGET)

$(TARGET):
	$(CC) -Wall -std=c99 -g -O0 `pkg-config --cflags glfw3` -o display $(MAIN) $(SOURCES) `pkg-config --static --libs glfw3`

ring_buffer_test: ring_buffer.c ring_buffer_test.c ring_buffer.h
	$(CC) -Wall -std=c99 -g -O0 ring_buffer.c ring_buffer_test.c -o ring_buffer_test

.PHONY: clean run
.DEFAULT: $(TARGET)
