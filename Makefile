PATH += :/usr/local/pspdev/psp/bin
CC=psp-gcc
LD=psp-ld
OUTPATCH=outpatch
CFLAGS=-g -G0 -mgp32 -mlong32 -O2 -c
OUT_EDITED=outp
OUT_BIN=out
OBJS=startup.o test.o matrix.o trig.o render.o raster.o

all: EBOOT.PBP

EBOOT.PBP: $(OUT_EDITED)
	pack-pbp EBOOT.PBP param.sfo icon0.png NULL NULL NULL NULL $(OUT_EDITED) NULL

$(OUT_EDITED): $(OUT_BIN)
	$(OUTPATCH) $(OUT_EDITED) $(OUT_BIN)

$(OUT_BIN): $(OBJS)
	$(LD) -O0 $(OBJS) -M -Ttext 8900000 -q -o $(OUT_BIN) > test.map

startup.o: startup.S
	$(CC) -g -G0 -mgp32 -c -xassembler -O -o startup.o startup.S

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	rm $(OBJS) $(OUT_BIN) $(OUT_EDITED) EBOOT.PBP

