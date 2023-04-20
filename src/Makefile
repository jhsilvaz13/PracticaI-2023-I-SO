CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic
SRCS=main.c interfaz.c search.c
OBJS=$(SRCS:.c=.o)
HDRS=interfaz.h search.h

all: programa

programa: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o programa

$(OBJS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f $(OBJS) programa