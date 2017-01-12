game: game.o alien.o helpers.o
	gcc -o game game.o alien.o helpers.o `pkg-config --libs sdl`

alien.o: alien.c alien.h helpers.h
	gcc -c -Wall -Wextra alien.c `pkg-config --cflags sdl`

game.o: game.c alien.h
	gcc -c -Wall -Wextra game.c `pkg-config --cflags sdl`

helpers.o: helpers.c 
	gcc -c -Wall -Wextra helpers.c

clean:
	rm game *.o
