# Compiler
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -g -D_POSIX_C_SOURCE=200809L

# Target executable name
TARGET  := my_curl

# Source and object files
SRC     := main.c #utils.c builtin_cmds.c
OBJ     := $(SRC:.c=.o)

# Headers
HEADERS := #utils.h builtin_cmds.h

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