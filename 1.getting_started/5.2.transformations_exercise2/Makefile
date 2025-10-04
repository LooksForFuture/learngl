build:
	mkdir -p bin/
	clang -std=c99 -Wall -Wextra -Wpedantic \
	      main.c ../../thirdparty/glad4.6/src/glad.c -o bin/game \
	      -I../../thirdparty/glad4.6/include -I../../thirdparty/cglm/include \
	      -I../../include/ \
	      -lglfw -lX11 -lXi -lXrandr -ldl -lm \
	      && cp -r shaders/ bin/ && cp -r assets/ bin/ \
	      && cd bin/ && ./game
