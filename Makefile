.DEFAULT_GOAL := build

.PHONY: build clean q2pro dday

q2pro:
	$(MAKE) -C q2proSRC
	cp -a q2proSRC/q2pro DDayNormandy
	cp -a q2proSRC/q2proded DDayNormandyded
	
dday/config.cfg:
	cp -a dday/config.cfg.sample dday/config.cfg

dday: dday/config.cfg
	echo "ARCH=`arch`" > variable.mk
	$(MAKE) -C DDaySRC
	cp -a DDaySRC/game?*.* dday/

build: q2pro dday

clean:
	$(MAKE) -C q2proSRC clean
	rm -f DDayNormandy
	rm -f DDayNormandyded
	$(MAKE) -C DDaySRC clean
	rm -f dday/game?*.*
	$(MAKE) -C DDaySRC/ai clean
	$(MAKE) -C DDaySRC/gbr clean
	$(MAKE) -C DDaySRC/grm clean
	$(MAKE) -C DDaySRC/ita clean
	$(MAKE) -C DDaySRC/jpn clean
	$(MAKE) -C DDaySRC/pol clean
	$(MAKE) -C DDaySRC/rus clean
	$(MAKE) -C DDaySRC/usa clean
	$(MAKE) -C DDaySRC/usm clean
	rm -f variable.mk

