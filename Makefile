CC=gcc
CFLAGS=-I.
DEPS = mydlp-beacon.h listifc.h
OBJ = mydlp-beacon.o listifc.o 

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

all: mydlp-beacon sender receiver

mydlp-beacon: $(OBJ)
		gcc -o $@ $^ $(CFLAGS)

sender: 
		gcc $@.c -o $@

receiver:
		gcc $@.c -o $@

clean:
		rm *.o sender receiver mydlp-beacon
