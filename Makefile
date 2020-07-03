build:
	g++ -w -std=c++14 \
	./src/*.cpp \
	-o game.exe \
	-IC:\SDL\SDL2\include \
	-LC:\SDL\SDL2\lib \
    -I.\lib\lua \
    -L.\lib\lua \
	-llua53 \
    -lmingw32 \
    -lSDL2main \
    -lSDL2_image \
    -lSDL2_ttf \
    -lSDL2_mixer \
    -lSDL2

clean:
	del game.exe

run:
	game.exe
