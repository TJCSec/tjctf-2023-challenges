all: clean hide_flag make_enc

.PHONY: clean

hide_flag:
	g++ hide_flag.cpp -o hide_flag -O3

make_enc:
	g++ make_enc.cpp -o make_enc -O3
	
clean:
	rm -f hide_flag
	rm -f make_enc
