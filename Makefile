ZIO ?= $(HOME)/zio
LIBZIO ?= $(HOME)/libzio

HOST_EXTRACFLAGS += -I$(ZIO)/include -I$(LIBZIO)
LIB := -static -L$(LIBZIO) -lzio

HOSTCC ?= gcc

PROGS := zio-dev-status

all: $(PROGS)

%: %.c 
	$(HOSTCC) $(HOST_EXTRACFLAGS) -Wall $^ -o $@ $(LIB)

clean:
	rm $(PROGS)

.PHONY: all, clean
