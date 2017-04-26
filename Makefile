.DEFAULT_GOAL := build

.PHONY: build clean q2pro dday

q2pro:
	$(MAKE) -C q2proSRC
	cp -a q2proSRC/q2pro .
	cp -a q2proSRC/q2proded .

dday/config.cfg:
	cp -a dday/config.cfg.sample dday/config.cfg

dday: dday/config.cfg
	$(MAKE) -C DDaySRC
	cp -a DDaySRC/game?*.* dday/

build: q2pro dday

clean:
	$(MAKE) -C q2proSRC clean
	rm -f q2pro
	rm -f q2proded
	$(MAKE) -C DDaySRC clean
	rm -f dday/game?*.*
