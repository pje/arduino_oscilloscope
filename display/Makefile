NAME=display
SOURCES=main.cpp renderarea.cpp ring_buffer.c window.cpp sample_producer.c arduino-serial-lib.c
HEADERS=renderarea.h ring_buffer.h window.h sample_producer.h arduino-serial-lib.h
TARGET=$(NAME)

$(NAME).pro: $(SOURCES) $(HEADERS)
	qmake -project -nopwd -Wall "TEMPLATE=app" "CONFIG+=static" "CONFIG-=app_bundle" "TARGET=$(NAME)" $(SOURCES) $(HEADERS)

Makefile.generated: $(NAME).pro
	qmake -makefile -Wall -o Makefile.generated

$(TARGET): Makefile.generated
	$(MAKE) -f Makefile.generated

clean:
	rm -rf *.o
	rm -rf moc_*.cpp
	rm -rf qrc_*.cpp
	rm -rf Makefile.generated
	rm -rf $(NAME).pro
	rm -rf $(TARGET)

run:
	./$(TARGET)

.PHONY: clean run
.DEFAULT: $(TARGET)

ring_buffer_test: ring_buffer.c ring_buffer_test.c ring_buffer.h
	gcc -Wall ring_buffer.c ring_buffer_test.c -o ring_buffer_test
