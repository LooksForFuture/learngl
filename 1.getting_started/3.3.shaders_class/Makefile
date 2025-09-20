build:
	mkdir -p bin/
	clang -std=c89 -Wall -Wextra -Wpedantic \
	      main.c ../../thirdparty/glad4.6/src/glad.c -o bin/game \
	      -I../../thirdparty/glad4.6/include -I../../include/ \
	      -lglfw -lX11 -lXi -lXrandr -ldl \
	      && cp -r assets/ bin/ && cd bin/ && ./game
