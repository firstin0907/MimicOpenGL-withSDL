
all: options.o mimic.o main.o per_sample_operator.o scan_converser.o fragment_processor.o vertex_processor.o vao_vbo.o
	g++ -I src/include -L src/lib -o main mimic.o main.o options.o per_sample_operator.o scan_converser.o fragment_processor.o vertex_processor.o vao_vbo.o -lmingw32 -lSDL2main -lSDL2

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

options.o: mimic/src/vao_vbo.cc
	g++ -I src/include -c -o options.o mimic/src/options.cc

clean:
	rm main.exe mimic.o main.o per_sample_operator.o scan_converser.o fragment_processor.o
	rm vertex_processor.o vao_vbo.o options.o