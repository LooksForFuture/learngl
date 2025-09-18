build:
	mkdir -p bin/
	clang -std=c89 -Wall -Wextra -Wpedantic \
		main.c ../../thirdparty/glad4.6/src/glad.c -o bin/game \
		-I../../thirdparty/glad4.6/include \
		-lglfw -lX11 -lXi -lXrandr -ldl \
		&& cd bin/ && ./game
