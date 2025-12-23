CC = gcc
CFLAGS = -g -Wall
SRCS = main.c continuation.c parse.c
OBJS = $(SRCS:.c=.o)
TARGET = scm

TEST_SRCS = test.c
TEST_OBJS_FILES = test.o main.test.o continuation.o parse.o
TEST_RUNNER = test_runner

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_RUNNER)
	./sample/test.sh
	./$(TEST_RUNNER)

$(TEST_RUNNER): $(TEST_OBJS_FILES)
	$(CC) $(CFLAGS) -o $@ $^

main.test.o: main.c
	$(CC) $(CFLAGS) -DTEST_BUILD -c $< -o $@

clean:
	rm -f $(TARGET) $(TEST_RUNNER) $(OBJS) test.o main.test.o

tags:
	ctags -R .
