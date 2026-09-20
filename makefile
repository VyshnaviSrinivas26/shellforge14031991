CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = src/lexer.c \
      src/token.c \
      src/parser.c \
      src/expand.c \
      src/main.c
TARGET = shellforge

all:
	$(CC) $(CFLAGS) $(SRC) -lreadline -o $(TARGET)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)
