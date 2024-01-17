# Makefile
CC = gcc
CFLAGS += -Wno-sign-compare -Wno-unused-function -Wall -Wextra -std=gnu11 -Wcomment -O3 -Ofast
LDFLAGS += -lgmp

# Optimization flag
OPT_LEVEL = $(DEFAULT_OPT_LEVEL)
DEFAULT_OPT_LEVEL = GENERIC


# Define parameters
EXECUTABLE0 = main64
EXECUTABLE1 = main128
EXECUTABLE2 = main192
EXECUTABLE3 = main256
EXECUTABLE4 = main512

# Basic source files
SRC = main.c random/random.c arith.c bench.c

# Define source files
ifeq ($(OPT_LEVEL),GENERIC)
SRC64 = $(SRC)  p64/generic/arith_generic.c
SRC128 = $(SRC) p128/generic/arith_generic.c
SRC192 = $(SRC) p192/generic/arith_generic.c
SRC256 = $(SRC) p256/generic/arith_generic.c
SRC512 = $(SRC) p512/generic/arith_generic.c
else ifeq ($(OPT_LEVEL),FAST)
SRC64 = $(SRC)  p64/ARM64/arith_arm64.c   p64/ARM64/arith_arm64.S
SRC128 = $(SRC) p128/ARM64/arith_arm128.c p128/ARM64/arith_arm128.S
SRC192 = $(SRC) p192/ARM64/arith_arm192.c p192/ARM64/arith_arm192.S
SRC256 = $(SRC) p256/ARM64/arith_arm256.c p256/ARM64/arith_arm256.S
SRC512 = $(SRC) p512/ARM64/arith_arm512.c p512/ARM64/arith_arm512.S
endif


all: main64 main128 main192 main256 main512


main64: $(SRC64)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=0 -o $@ $^

main128: $(SRC128)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=1 -o $@ $^

main192: $(SRC192)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=2 -o $@ $^

main256: $(SRC256)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=3 -o $@ $^

main512: $(SRC512)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=4 -o $@ $^


clean:
	rm -f main64 main128 main192 main256 main512





