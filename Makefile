CC=clang

run: main
		bin/main

main: main.c init
		$(CC) -I$(INC_PATH) -o bin/main main.c -L$(LIB_PATH) -lsdl2 -lsdl2_image

init:
		mkdir -p bin/

clean:
		rm -r bin/
