SOURCES:=$(shell find src -name "*.c")
OBJECTS:=$(SOURCES:src/%.c=bin/%.o)


all:main flash

flash: main
	msp430-flash main

.PHONY: flash

main: $(OBJECTS)
	msp430-gcc -std=c99 -mmcu=msp430f5529 -mdisable-watchdog -o $@ $^

clean:
	rm -rf main

	
bin/%.o: src/%.c src/lib/*.h src/*.h
	mkdir -p `dirname $@`
	msp430-gcc -std=c99 -mmcu=msp430f5529 -mdisable-watchdog -o $@ -c $< 