NAME=display
SRC_FILES=main.cpp renderarea.cpp renderarea.h window.cpp window.h

$(NAME).pro: $(SRC_FILES)
	qmake -project TEMPLATE=app CONFIG+=static TARGET=$(NAME) -Wall

Makefile.generated: $(NAME).pro
	qmake -makefile -o Makefile.generated -Wall

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

.PHONY: clean
.DEFAULT: $(NAME).app
