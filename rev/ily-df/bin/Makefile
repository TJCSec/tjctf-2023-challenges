.PHONY: clean all

all: clean game clean gen_world

game:
	gcc gen.c -o gen_world
	./gen_world > world.h
	gcc main.c -O -o game -lcurses
	strip game

clean:
	rm -f game
	rm -f gen_world