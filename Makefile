# Makefile for Game of Life

all:game_of_life

game_of_life:
	g++ -o main.out ./src/main.cpp

clean:
	rm -f main