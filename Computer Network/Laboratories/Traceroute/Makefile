CC = gcc
CFLAGS = -std=c17 -Wall -Wextra

SRC = traceroute.c sending.c receiving.c aux_func.c
OBJ = $(SRC:.c=.o)
EXEC = traceroute

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

distclean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
