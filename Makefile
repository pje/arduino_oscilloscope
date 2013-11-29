run:
	cd input && $(MAKE) clean upload
	cd display && $(MAKE) clean display run

.PHONY: all
.DEFAULT: all
