
all: mimic.o main.o
	g++ -I src/include -L src/lib -o main main.o mimic.o -lmingw32 -lSDL2main -lSDL2

main.o: main.cc
	g++ -I src/include -c -o main.o main.cc
	
mimic.o: mimic/src/mimic.cc
	g++ -I src/include -c -o mimic.o mimic/src/mimic.cc