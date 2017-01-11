game: game.o
	gcc -o game game.o `pkg-config --libs sdl`

game.o: spacexplore.c
	gcc -c -Wall -Wextra game.c `pkg-config --cflags sdl`

clean:
	rm game *.o
