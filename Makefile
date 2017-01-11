game: game.o alien.o
	gcc -o game game.o alien.o `pkg-config --libs sdl`

alien.o: alien.c alien.h
	gcc -c -Wall -Wextra alien.c `pkg-config --cflags sdl`

game.o: game.c alien.h
	gcc -c -Wall -Wextra game.c `pkg-config --cflags sdl`

clean:
	rm game *.o
