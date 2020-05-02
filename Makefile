build:
	gcc -Wfatal-errors \
	-std=c99 \
	./src/*.c \
	-I"C:/libsdl/include" \
	-L"C:/libsdl/lib" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-o 3drenderer.exe

run:
	3drenderer.exe

clean:
	del 3drenderer.exe