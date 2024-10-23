# Makefile
CC = gcc
CFLAGS += -Wno-sign-compare -Wno-unused-function -Wall -Wextra -std=gnu11 -Wcomment -O3 -Ofast
CFLAGS += -I/usr/local/include
LDFLAGS += -L/usr/local/lib
LIBS += -lblake3

# CFLAGS += -g -fsanitize=address -ferror-limit=1 		// For debugging

.NOTPARALLEL:

# Optimization flag
OPT_LEVEL = $(DEFAULT_OPT_LEVEL)
DEFAULT_OPT_LEVEL = GENERIC

# Alternative primes
ifeq ($(ALT_PRIMES),ALT)
PRIMES_VAL=1
else
PRIMES_VAL=0
endif


# Basic source files
SRC = random/random.c arith.c bench.c


# Add arithmetic source files
ifeq ($(OPT_LEVEL),GENERIC)
SRC64 = $(SRC)  p64/generic/arith_generic.c
SRC128 = $(SRC) p128/generic/arith_generic.c
SRC192 = $(SRC) p192/generic/arith_generic.c
SRC256 = $(SRC) p256/generic/arith_generic.c
SRC512 = $(SRC) p512/generic/arith_generic.c
else ifeq ($(OPT_LEVEL),FAST)
SRC64 = $(SRC)  p64/arm64/arith_arm64.c   p64/arm64/arith_arm64.S
SRC128 = $(SRC) p128/arm64/arith_arm128.c p128/arm64/arith_arm128.S
SRC192 = $(SRC) p192/arm64/arith_arm192.c p192/arm64/arith_arm192.S
SRC256 = $(SRC) p256/arm64/arith_arm256.c p256/arm64/arith_arm256.S
SRC512 = $(SRC) p512/arm64/arith_arm512.c p512/arm64/arith_arm512.S
endif


.PHONY: all dOPRF arith clean

all: dOPRF arith


dOPRF: dOPRF64 dOPRF128 dOPRF192 dOPRF256 dOPRF512

arith: arith64 arith128 arith192 arith256 arith512


dOPRF64: main.c $(SRC64) dOPRF.c
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=0 -DPRIMES=$(PRIMES_VAL) -o $@ $^ $(LIBS)

dOPRF128: main.c $(SRC128) dOPRF.c
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=1 -DPRIMES=$(PRIMES_VAL) -o $@ $^ $(LIBS)

dOPRF192: main.c $(SRC192) dOPRF.c
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=2 -DPRIMES=$(PRIMES_VAL) -o $@ $^ $(LIBS)

dOPRF256: main.c $(SRC256) dOPRF.c
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=3 -DPRIMES=$(PRIMES_VAL) -o $@ $^ $(LIBS)

dOPRF512: main.c $(SRC512) dOPRF.c
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=4 -DPRIMES=$(PRIMES_VAL) -o $@ $^ $(LIBS)


arith64: arith_tests.c $(SRC64)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=0 -DPRIMES=$(PRIMES_VAL) -o $@ $^ $(LIBS)

arith128: arith_tests.c $(SRC128)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=1 -DPRIMES=$(PRIMES_VAL) -o $@ $^ $(LIBS)

arith192: arith_tests.c $(SRC192)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=2 -DPRIMES=$(PRIMES_VAL) -o $@ $^ $(LIBS)

arith256: arith_tests.c $(SRC256)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=3 -DPRIMES=$(PRIMES_VAL) -o $@ $^ $(LIBS)

arith512: arith_tests.c $(SRC512)
	$(CC) $(CFLAGS) $(LDFLAGS)  -DSEC_LEVEL=4 -DPRIMES=$(PRIMES_VAL) -o $@ $^ $(LIBS)


clean:
	rm -f dOPRF64 dOPRF128 dOPRF192 dOPRF256 dOPRF512 arith64 arith128 arith192 arith256 arith512



