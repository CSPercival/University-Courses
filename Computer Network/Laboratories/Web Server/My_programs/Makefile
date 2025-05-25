CC = gcc
CFLAGS = -std=c17 -Wall -Wextra
LDFLAGS = -lm

SRC = server.c aux.c setup.c receive.c send.c
OBJ = $(SRC:.c=.o)
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

distclean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean distclean
