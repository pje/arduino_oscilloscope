everything:
	cd producer && $(MAKE) clean upload
	cd consumer && $(MAKE) clean consumer run

build: build_producer build_consumer

clean: clean_producer clean_consumer

run: run_producer run_consumer

build_consumer:
	cd consumer && $(MAKE) build

clean_consumer:
	cd consumer && $(MAKE) clean

run_consumer:
	cd consumer && $(MAKE) run

build_producer:
	cd producer && $(MAKE) upload

clean_producer:
	cd producer && $(MAKE) clean

run_producer:
	cd producer && $(MAKE) run

.PHONY: build clean build_consumer clean_consumer run_consumer build_producer clean_producer run_producer run
.DEFAULT: everything
