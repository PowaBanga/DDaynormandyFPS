.DEFAULT_GOAL := build

.PHONY: build clean q2pro dday q2admin

q2pro:
	$(MAKE) -C src/q2pro CONFIG_FILE=../q2pro.config
	cp -a src/q2pro/q2pro ddaynormandy
	cp -a src/q2pro/q2proded ddaynormandyded
	
dday/config.cfg:
	cp -a dday/config.cfg.sample dday/config.cfg

dday: dday/config.cfg
	$(MAKE) -C src/dday
	cp -a src/dday/game?*.real.* dday/

q2admin:
	$(MAKE) -C src/q2admin-tsmod
	cp -a src/q2admin-tsmod/game?*.* dday/

build: q2pro dday q2admin

install:
	mkdir -p $(DESTDIR)$(CONFIG_PATH_LIB)
	mkdir -p $(DESTDIR)$(CONFIG_PATH_DATA)/dday
	cp -a src/q2pro/q2pro $(DESTDIR)$(CONFIG_PATH_LIB)/ddaynormandy
	cp -a src/q2pro/q2proded $(DESTDIR)$(CONFIG_PATH_LIB)/ddaynormandyded
	cp -a src/dday/game?*.real.* $(DESTDIR)$(CONFIG_PATH_DATA)/dday/
	cp -a dday/config.cfg.sample $(DESTDIR)$(CONFIG_PATH_DATA)/dday/config.cfg
	cp -a src/q2admin-tsmod/game?*.* $(DESTDIR)$(CONFIG_PATH_DATA)/dday/
	cp -ar dday/* $(DESTDIR)$(CONFIG_PATH_DATA)/dday/

clean:
	$(MAKE) -C src/q2pro clean
	rm -f ddaynormandy
	rm -f ddaynormandyded
	$(MAKE) -C src/dday clean
	rm -f dday/game?*.real.*
	$(MAKE) -C src/q2admin-tsmod clean
	rm -f dday/game?*.*

