# Compiler
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -g

# Target executable name
TARGET  := tsh

# Source and object files
SRC     := tsh.c utils.c builtin_cmds.c
OBJ     := $(SRC:.c=.o)

# Headers
HEADERS := utils.h builtin_cmds.h

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

fclean: clean
	rm -f $(TARGET)
	rm -rf tests/actual tests/expected runner backup.txt

re: fclean all