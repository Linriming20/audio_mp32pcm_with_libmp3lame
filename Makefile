src := main.c
static_lib := lib/libmp3lame.a

CC := gcc
CFLAGS := -I./include
LDFLAGS := -lm

ifeq ($(DEBUG), 1)
CFLAGS += -DENABLE_DEBUG
endif

all: mp32pcm


mp32pcm: $(src) $(static_lib)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f mp32pcm out*
.PHONY := clean
