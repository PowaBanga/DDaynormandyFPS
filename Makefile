.DEFAULT_GOAL := build

.PHONY: build clean q2pro dday q2admin

q2pro:
	$(MAKE) -C src/q2pro CONFIG_FILE=../q2pro.config
	cp -a src/q2pro/q2pro ddaynormandy
	cp -a src/q2pro/q2proded ddaynormandy-dedicated
	
dday/config.cfg:
	cp -a dday/config.cfg.sample dday/config.cfg

dday: dday/config.cfg
	$(MAKE) -C src/dday
	cp -a src/dday/game?*.real.* dday/

q2admin:
	$(MAKE) -C src/q2admin-tsmod
	cp -a src/q2admin-tsmod/game?*.* dday/

build: q2pro dday q2admin

clean:
	$(MAKE) -C src/q2pro clean
	rm -f ddaynormandy
	rm -f ddaynormandy-dedicated
	$(MAKE) -C src/dday clean
	rm -f dday/game?*.real.*
	$(MAKE) -C src/dday/ai clean
	$(MAKE) -C src/dday/gbr clean
	$(MAKE) -C src/dday/grm clean
	$(MAKE) -C src/dday/ita clean
	$(MAKE) -C src/dday/jpn clean
	$(MAKE) -C src/dday/pol clean
	$(MAKE) -C src/dday/rus clean
	$(MAKE) -C src/dday/usa clean
	$(MAKE) -C src/dday/usm clean
	$(MAKE) -C src/q2admin-tsmod clean
	rm -f dday/game?*.*

