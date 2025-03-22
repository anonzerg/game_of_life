CC := clang
CFLAGS := -O2 -Wall -Wextra $(shell sdl2-config --cflags) $(shell sdl2-config --cflags)
LIBS := $(shell sdl2-config --libs) $(shell gsl-config --libs)
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:.c=.o)
TRAGET = game_of_life

./bin/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

./bin/%.o: ./src/%.c ./bin
	$(CC) $(CFLAGS) -c $< -o $@

./bin:
	mkdir ./bin

clean:
	rm -rf ./src/*.o
	rm -rf ./bin/*

