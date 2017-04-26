.DEFAULT_GOAL := build

.PHONY: build clean q2pro dday
ARCH=$(arch)
q2pro:
	$(MAKE) -C q2proSRC
	cp -a q2proSRC/q2pro .
	cp -a q2proSRC/q2proded .
	mv q2pro DDaynormandy
	mv q2proded Dediedserver
dday/config.cfg:
	cp -a dday/config.cfg.sample dday/config.cfg

dday: dday/config.cfg
	$(MAKE) -C DDaySRC
	cp -a DDaySRC/game?*.* dday/

build: q2pro dday

clean:
	$(MAKE) -C q2proSRC clean
	rm -f DDaynormandy
	rm -f Dediedserver
	$(MAKE) -C DDaySRC clean
	rm -f dday/game?*.*
