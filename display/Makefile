NAME=display
SOURCES=main.cpp renderarea.cpp ring_buffer.c window.cpp
HEADERS=renderarea.h ring_buffer.h window.h

$(NAME).pro: $(SOURCES) $(HEADERS)
	qmake -project -nopwd -Wall "TEMPLATE=app" "CONFIG+=static" "TARGET=$(NAME)" $(SOURCES) $(HEADERS)

Makefile.generated: $(NAME).pro
	qmake -makefile -Wall -o Makefile.generated

$(NAME).app: Makefile.generated
	$(MAKE) -f Makefile.generated

clean:
	rm -rf *.o
	rm -rf moc_*.cpp
	rm -rf qrc_*.cpp
	rm -rf Makefile.generated
	rm -rf $(NAME).pro
	rm -rf $(NAME).app

run:
	open $(NAME).app

.PHONY: clean run
.DEFAULT: $(NAME).app

ring_buffer_test:
	gcc -Wall ring_buffer.c ring_buffer_test.c -o ring_buffer_test
