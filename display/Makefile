PROJECT_ROOT=$(CURDIR)
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

ring_buffer_test: ring_buffer.c ring_buffer_test.c ring_buffer.h
	gcc -Wall ring_buffer.c ring_buffer_test.c -o ring_buffer_test

.PHONY: clean run qt-static
.DEFAULT: $(TARGET)

QT_ARCHIVE_URL=http://download.qt-project.org/official_releases/qt/4.8/4.8.5/qt-everywhere-opensource-src-4.8.5.tar.gz
QT_ARCHIVE_FILE=$(PROJECT_ROOT)/tmp/qt-everywhere-opensource-src-4.8.5.tar.gz
QT_SRC_DIR_NAME=qt-everywhere-opensource-src-4.8.5
QT_SRC_DIR=$(PROJECT_ROOT)/vendor/$(QT_SRC_DIR_NAME)

$(QT_ARCHIVE_FILE):
	mkdir -p $(PROJECT_ROOT)/tmp
	curl --fail -L $(QT_ARCHIVE_URL) -o $@

$(QT_SRC_DIR): $(QT_ARCHIVE_FILE)
	mkdir -p $(PROJECT_ROOT)/vendor
	cd $(PROJECT_ROOT)/vendor && tar xfz $(QT_ARCHIVE_FILE)

qt-static:
	mkdir -p $(PROJECT_ROOT)/vendor/qt-static
	cd $(QT_SRC_DIR) && ./configure -opensource -release -static -fast -prefix $(PROJECT_ROOT)/vendor/qt-static -no-largefile -no-system-proxies -no-stl -no-sql-mysql -no-qt3support -no-xmlpatterns -no-multimedia -no-audio-backend -no-phonon -no-phonon-backend -no-svg -no-webkit -no-javascript-jit -no-script -no-scripttools -no-declarative -no-declarative-debug -no-dwarf2 -no-separate-debug-info -no-gif -no-libtiff -no-libpng -no-libmng -no-libjpeg -no-openssl -no-nis -no-cups -no-iconv -no-dbus -nomake libs  -nomake tools -nomake examples -nomake demos -nomake docs -nomake translations
	cd $(QT_SRC_DIR) && make sub-src
