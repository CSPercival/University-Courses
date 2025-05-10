CC = gcc
CFLAGS = -std=c17 -Wall -Wextra

SRC = main.c aux.c setup.c
OBJ = $(SRC:.c=.o)
EXEC = client

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
