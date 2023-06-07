.PHONY: all clean

all: clean chall

chall:
	gcc chall.c -o chall -fno-stack-protector -no-pie -z execstack -Wno-stringop-overflow

clean:
	rm -f chall
