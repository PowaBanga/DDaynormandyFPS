DESTDIR = /usr/local

.DEFAULT_GOAL := root

.PHONY: build clean q2proroot ddayroot q2admintsmodroot

q2pro:
	$(MAKE) -C q2proSRC
	cp -a q2proSRC/q2pro DDayNormandy
	cp -a q2proSRC/q2proded DDayNormandyded
	
dday/config.cfg:
	cp -a dday/config.cfg.sample dday/config.cfg

dday: dday/config.cfg
	echo "ARCH=$(shell arch)" > variable.mk
	$(MAKE) -C DDaySRC
	cp -a DDaySRC/game?*.real.* dday/

q2admintsmod:
	$(MAKE) -C q2admintsmod
	cp -a q2admintsmod/game?*.* dday/
CompAll:
	echo "ARCH=$(uname -m)" > variable.mk
	sed -i 's/#CONFIG_PATH/CONFIG_PATH/g' q2proSRC/.config
	$(MAKE) -C q2proSRC
	sed -i 's/CONFIG_PATH/#CONFIG_PATH/g' q2proSRC/.config
	$(MAKE) -C DDaySRC
	$(MAKE) -C q2admintsmod
install:
	mkdir $(DESTDIR)/lib/games/ddaynormandy
	mkdir $(DESTDIR)/share/games/ddaynormandy/dday
	cp -a q2proSRC/q2pro $(DESTDIR)/lib/games/ddaynormandy/ddaynormandy
	cp -a q2proSRC/q2proded $(DESTDIR)/lib/games/ddaynormandy/ddaynormandyded
	cp -a DDaySRC/game?*.real.* $(DESTDIR)/share/games/ddaynormandy/dday/
	cp -a dday/config.cfg.sample $(DESTDIR)/share/games/ddaynormandy/dday/config.cfg
	cp -a q2admintsmod/game?*.* $(DESTDIR)/share/games/ddaynormandy/dday/

build: q2pro dday q2admintsmod
root: CompAll

clean:
	$(MAKE) -C q2proSRC clean
	rm -f DDayNormandy
	rm -f DDayNormandyded
	$(MAKE) -C DDaySRC clean
	rm -f dday/game?*.real.*
	rm -f variable.mk
	$(MAKE) -C DDaySRC/ai clean
	$(MAKE) -C DDaySRC/gbr clean
	$(MAKE) -C DDaySRC/grm clean
	$(MAKE) -C DDaySRC/ita clean
	$(MAKE) -C DDaySRC/jpn clean
	$(MAKE) -C DDaySRC/pol clean
	$(MAKE) -C DDaySRC/rus clean
	$(MAKE) -C DDaySRC/usa clean
	$(MAKE) -C DDaySRC/usm clean
	$(MAKE) -C q2admintsmod clean
	rm -f dday/game?*.*

