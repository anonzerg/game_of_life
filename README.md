# [game_of_life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
Conway's game of life implemented in C using [gsl](https://www.gnu.org/software/gsl/) and [sdl2](https://www.libsdl.org/).

## how to build
install `make`, `clang`, `libgsl-dev` and `libsdl2-dev`
```
sudo apt install make clang libgsl-dev libsdl2-dev
```
clone repo and cd into it
```
git clone https://github.com/anonzerg/game_of_life.git
cd game_of_life
```
use make to compile project
```
make
```
and run the program by
```
./bin/game_of_life
```

> [!NOTE]
> this is a simple try to use with SDL2 and GSL in a program and not a complete program.

