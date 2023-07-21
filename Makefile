OBJS = options.o mimic.o main.o per_sample_operator.o scan_converser.o\
	fragment_processor.o vertex_processor.o vao_vbo.o

INCLUDE_DIR = src/include
SRC_DIR = mimic/src

all: $(OBJS)
	g++ -I $(INCLUDE_DIR) -L src/lib -o main $(OBJS) -lmingw32 -lSDL2main -lSDL2

main.o: main.cc
	g++ -I $(INCLUDE_DIR) -c -o main.o main.cc
	
mimic.o: $(SRC_DIR)/mimic.cc
	g++ -I $(INCLUDE_DIR) -c -o mimic.o $(SRC_DIR)/mimic.cc

per_sample_operator.o: $(SRC_DIR)/per_sample_operator.cc
	g++ -I $(INCLUDE_DIR) -c -o per_sample_operator.o $(SRC_DIR)/per_sample_operator.cc

scan_converser.o: $(SRC_DIR)/scan_converser.cc
	g++ -I $(INCLUDE_DIR) -c -o scan_converser.o $(SRC_DIR)/scan_converser.cc

fragment_processor.o: $(SRC_DIR)/fragment_processor.cc
	g++ -I $(INCLUDE_DIR) -c -o fragment_processor.o $(SRC_DIR)/fragment_processor.cc

vertex_processor.o: $(SRC_DIR)/vertex_processor.cc
	g++ -I $(INCLUDE_DIR) -c -o vertex_processor.o $(SRC_DIR)/vertex_processor.cc

vao_vbo.o: $(SRC_DIR)/vao_vbo.cc
	g++ -I $(INCLUDE_DIR) -c -o vao_vbo.o $(SRC_DIR)/vao_vbo.cc

options.o: $(SRC_DIR)/vao_vbo.cc
	g++ -I $(INCLUDE_DIR) -c -o options.o $(SRC_DIR)/options.cc

.PHONY: clean
clean:
	-del $(OBJS) main.exe