lib: messend.o
	clang -shared build/messend.o -o build/libmessend.so -lSDL2 -lSDL2_net

messend.o: messend.c messend.h
	clang -std=c99 -c messend.c -o build/messend.o

