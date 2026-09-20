CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LIBS = -lreadline

SOURCES = src/expand.c src/lexer.c src/main.c src/parser.c src/token.c
TARGET = shellforge

all:
	$(CC) $(CFLAGS) $(SOURCES) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
