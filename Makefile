PATH += :/usr/local/pspdev/bin
CC=psp-gcc
LD=psp-ld
OUTPATCH=outpatch
CFLAGS=-G0 -O3 -c -Wall
#squish some builtins
CFLAGS += -fno-builtin-sin -fno-builtin-cos -fno-builtin-tan -fno-builtin-sqrt
OUT_EDITED=outp
OUT_BIN=out
OBJS=matrix.o math.o render.o raster.o malloc.o

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $<

#other util stuff
clean:
	rm $(OBJS)

