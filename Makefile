CC=clang
CFLAGS=`pkg-config --cflags sdl2 sdl2_image`
LIBS=`pkg-config --libs sdl2 sdl2_image`

run: main
		bin/main

main: main.c init
		$(CC) $(CFLAGS) -O3 -o bin/main main.c $(LIBS)

init:
		mkdir -p bin/

clean:
		rm -r bin/
