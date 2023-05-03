CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -framework ApplicationServices -framework Foundation -framework AppKit

all: slurp

slurp: slurp.c clip.m
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f slurp

