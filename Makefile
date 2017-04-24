.DEFAULT_GOAL := build

q2pro:
	$(MAKE) -C q2proSRC
	cp -a q2proSRC/q2pro .
	cp -a q2proSRC/q2proded .

dday:
	$(MAKE) -C DDaySRC
	cp -a DDaySRC/game*.* dday/

build: q2pro dday

clean:
	$(MAKE) -C q2proSRC clean
	$(MAKE) -C DDaySRC clean
