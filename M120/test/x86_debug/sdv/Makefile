
PRJ_TYPE = gcc

CFLAGS = -fPIC -O -Wall -c

RM := /bin/rm -fr

LDFLAGS := -shared -lft4222 -lSUSI-4.00 -L./

TARGET_LIB = libsdv.so

SRC  := ./
SSRC := $(shell find $(SRC) -name '*.c')
SOBJS := $(SSRC:%.c=%.o)
COMMON_OBJS = $(patsubst %.c,%.o,./jtag/*.c,./poe/*.c)
INCLUDEPATH = -I$(PWD)/jtag -I$(PWD)/poe

all: $(SOBJS)
	$(LD) $(LDFLAGS) -fPIC -o $(TARGET_LIB) $(SOBJS)

%.o : %.c
	$(PRJ_TYPE) $(CFLAGS) $(INCLUDEPATH) -o $@ $<

clean:
	-$(RM) $(TARGET_LIB) $(SOBJS) $(COMMON_OBJS)

.PHONY : all clean
