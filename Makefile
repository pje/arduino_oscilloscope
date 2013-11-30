everything:
	cd producer && $(MAKE) clean upload
	cd consumer && $(MAKE) clean consumer run

build: producer_build consumer_build

clean: producer_clean consumer_clean

run: producer_run consumer_run

consumer_build:
	cd consumer && $(MAKE) build

consumer_clean:
	cd consumer && $(MAKE) clean

consumer_run:
	cd consumer && $(MAKE) run

producer_build:
	cd producer && $(MAKE) upload

producer_clean:
	cd producer && $(MAKE) clean

producer_run:
	cd producer && $(MAKE) run

.PHONY: build clean consumer_build consumer_clean consumer_run producer_build producer_clean producer_run run
.DEFAULT: everything
