ARDUINO_DIR  = /Users/patrickellis/Applications/Arduino.app/Contents/Resources/Java
ARDMK_DIR    = $(CURDIR)/Arduino-Makefile
ARDMK_PATH	 = $(CURDIR)/Arduino-Makefile/bin
ARDUINO_LIBS =
BOARD_TAG		 = atmega328
MONITOR_PORT = /dev/tty.usbserial-A600afNY

$(CURDIR)/Arduino-Makefile:
	git clone https://github.com/sudar/Arduino-Makefile $@

-include $(ARDMK_DIR)/arduino-mk/Arduino.mk
