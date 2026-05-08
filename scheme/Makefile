CC = gcc
CFLAGS = -g -Wall
SRCS = main.c continuation.c parse.c repl.c value.c env.c memory.c eval.c ifunc.c
OBJS = $(SRCS:.c=.o)
TARGET = scm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: all
	./sample/test.sh
	./test/test.sh

clean:
	rm -f $(TARGET) $(OBJS)

tags:
	ctags -R .
