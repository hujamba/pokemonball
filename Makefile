game: game.o alien.o helpers.o text.o
	gcc -o game game.o alien.o helpers.o text.o `pkg-config --libs sdl SDL_ttf`

alien.o: alien.c alien.h helpers.h
	gcc -c -Wall -Wextra alien.c `pkg-config --cflags sdl`

game.o: game.c alien.h
	gcc -c -Wall -Wextra game.c `pkg-config --cflags sdl`

helpers.o: helpers.c 
	gcc -c -Wall -Wextra helpers.c `pkg-config --cflags sdl`

text.o: text.c
	gcc -c -Wall -Wextra text.c `pkg-config --cflags sdl SDL_ttf`

clean:
	rm game *.o
