CC=@gcc
CFLAGS=-I. -Wno-packed-bitfield-compat
DEPS = iformat.h  jformat.h  mipssim.h  rformat.h
OBJ = iformat.o  jformat.o  mipssim.o  rformat.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mipssim: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ mipssim
