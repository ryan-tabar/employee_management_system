CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lsqlite3

SRCS = main.c
OBJS = $(SRCS:.c=.o)
HEADER =
TARGET = program

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) database.db
