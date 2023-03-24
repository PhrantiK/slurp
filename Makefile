CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -framework ApplicationServices

all: slurp

slurp: slurp.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f slurp

