CC = gcc
CFLAGS = -std=c17 -Wall -Wextra

SRC = main.c setup.c aux.c receiving.c sending.c maintenance.c
OBJ = $(SRC:.c=.o)
EXEC = router

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
