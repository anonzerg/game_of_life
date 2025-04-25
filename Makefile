CC := clang
CFLAGS := -O2 -Wall -Wextra $(shell sdl2-config --cflags) $(shell gsl-config --cflags)
LIBS := $(shell sdl2-config --libs) $(shell gsl-config --libs)
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/.c=bin/.o)
TARGET = game_of_life

bin/$(TARGET): $(OBJS) | bin
	$(CC) $^ -o $@ $(LIBS)

bin/%.o: src/%.c | bin
	$(CC) $(CFLAGS) -c $< -o $@

bin:
	mkdir -p bin

clean:
	rm -rf ./bin/*

