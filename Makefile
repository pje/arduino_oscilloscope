everything:
	cd producer && $(MAKE) clean upload
	cd consumer && $(MAKE) clean consumer run

consumer_build:
	cd consumer && $(MAKE) build

consumer_clean:
	cd consumer && $(MAKE) clean

consumer_run:
	cd consumer && $(MAKE) run

producer_build:
	cd producer && $(MAKE) build

producer_clean:
	cd producer && $(MAKE) clean

producer_run:
	cd producer && $(MAKE) run


.PHONY: consumer_build consumer_clean consumer_run producer_build producer_clean producer_run run
.DEFAULT: everything
