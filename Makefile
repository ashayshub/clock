CC = gcc
# CFLAGS = -Wall -Werror -Wmissing-prototypes
CFLAGS = -Wall -Wmissing-prototypes

all: clock.h clock.c
	${CC} ${CFLAGS} -o clock clock.c -lncurses -lm

clean:
	rm -f clock
