###
##PRJ_TYPE = powerpc-wrs-linux-gnu-ppc_e300c2-glibc_small-gcc
PRJ_TYPE = gcc

CFLAGS_NORMAL = -Wall -O2 -DDISABLE_OPLK_DEBUG=1 -c

PRJ_NAME = cmd

RM := /bin/rm -fr

LIBS := -ldl -lpthread -lft4222 -lSUSI-4.00 -lsdv -L./
#MACRO := -D_GNU_SOURCE -D_DEBUG_OPEN
MACRO := -D_GNU_SOURCE

SRC  := $(wildcard *.c)  
SOBJS := $(patsubst %.c,%.o,$(SRC))

INCLUDEPATH = -I$(PWD)/../sdv -I$(PWD)/../sdv/jtag -I$(PWD)/../sdv/poe

all: $(PRJ_NAME)

$(PRJ_NAME): $(SOBJS)
	$(PRJ_TYPE) -o $@ $^ $(LIBS)

%.o : %.c
	$(PRJ_TYPE) $(CFLAGS_NORMAL) $(MACRO) $(INCLUDEPATH) -o $@ $<

clean:
	-$(RM) $(PRJ_NAME) $(SOBJS)

.PHONY : all clean
