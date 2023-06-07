.PHONY: all clean

# all: clean library chall
all: clean chall

library:
	gcc -o libfmtstr.so -shared -fPIC library.c

chall:
	# gcc -o chall chall.c -no-pie -L. -lfmtstr
	gcc -o chall chall.c -Wl,-z,norelro -no-pie -Wno-format-security -fno-stack-protector

clean:
	rm -f libfmtstr.so chall
