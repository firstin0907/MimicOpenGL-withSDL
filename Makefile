
all: mimic.o main.o per_sample_operator.o scan_converser.o fragment_processor.o vertex_processor.o vao_vbo.o
	g++ -I src/include -L src/lib -o main mimic.o main.o per_sample_operator.o scan_converser.o fragment_processor.o vertex_processor.o vao_vbo.o -lmingw32 -lSDL2main -lSDL2

main.o: main.cc
	g++ -I src/include -c -o main.o main.cc
	
mimic.o: mimic/src/mimic.cc
	g++ -I src/include -c -o mimic.o mimic/src/mimic.cc

per_sample_operator.o: mimic/src/per_sample_operator.cc
	g++ -I src/include -c -o per_sample_operator.o mimic/src/per_sample_operator.cc

scan_converser.o: mimic/src/scan_converser.cc
	g++ -I src/include -c -o scan_converser.o mimic/src/scan_converser.cc

fragment_processor.o: mimic/src/fragment_processor.cc
	g++ -I src/include -c -o fragment_processor.o mimic/src/fragment_processor.cc

vertex_processor.o: mimic/src/vertex_processor.cc
	g++ -I src/include -c -o vertex_processor.o mimic/src/vertex_processor.cc

vao_vbo.o: mimic/src/vao_vbo.cc
	g++ -I src/include -c -o vao_vbo.o mimic/src/vao_vbo.cc

clean:
	rm mimic.o main.exe main.o per_sample_operator.o