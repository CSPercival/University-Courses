CC = gcc
CFLAGS = -std=c17 -Wall -Wextra
LDFLAGS = -lm

SRC = main.c aux.c setup.c application.c receive.c send.c
OBJ = $(SRC:.c=.o)
EXEC = client

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
