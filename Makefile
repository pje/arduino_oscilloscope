run:
	cd producer && $(MAKE) clean upload
	cd consumer && $(MAKE) clean consumer run

.PHONY: all
.DEFAULT: all
