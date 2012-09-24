ZIO ?= $(HOME)/zio
LIBZIO ?= $(HOME)/libzio

HOST_EXTRACFLAGS += -I$(ZIO)/include -I$(LIBZIO)
LIB := -static -L$(LIBZIO) -lzio
DEBUG_CFLAGS := -g -ggdb -O0

HOSTCC ?= gcc

PROGS := lszio

all: $(PROGS)

%: %.c 
	$(HOSTCC) $(HOST_EXTRACFLAGS) $(DEBUG_CFLAGS) -Wall $^ -o $@ $(LIB)

clean:
	rm $(PROGS)

.PHONY: all, clean
