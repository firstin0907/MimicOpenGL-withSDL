OBJS = options.o mimic.o main.o per_sample_operator.o scan_converser.o\
	fragment_processor.o vertex_processor.o vao_vbo.o

INCLUDE_DIR = src/include
SRC_DIR     = mimic/src
BUILD_DIR   = build

OBJ_FILES = $(addprefix $(BUILD_DIR)/, $(OBJS))


all: $(OBJ_FILES)
	g++ -I $(INCLUDE_DIR) -L src/lib -o main $(OBJ_FILES) -lmingw32 -lSDL2main -lSDL2
	echo $(OBJ_FILES)

$(BUILD_DIR)/main.o: main.cc
	g++ -I $(INCLUDE_DIR) -c -o $(BUILD_DIR)/main.o main.cc
	
$(BUILD_DIR)/mimic.o: $(SRC_DIR)/mimic.cc
	g++ -I $(INCLUDE_DIR) -c -o $(BUILD_DIR)/mimic.o $(SRC_DIR)/mimic.cc

$(BUILD_DIR)/per_sample_operator.o: $(SRC_DIR)/per_sample_operator.cc
	g++ -I $(INCLUDE_DIR) -c -o $(BUILD_DIR)/per_sample_operator.o $(SRC_DIR)/per_sample_operator.cc

$(BUILD_DIR)/scan_converser.o: $(SRC_DIR)/scan_converser.cc
	g++ -I $(INCLUDE_DIR) -c -o $(BUILD_DIR)/scan_converser.o $(SRC_DIR)/scan_converser.cc

$(BUILD_DIR)/fragment_processor.o: $(SRC_DIR)/fragment_processor.cc
	g++ -I $(INCLUDE_DIR) -c -o $(BUILD_DIR)/fragment_processor.o $(SRC_DIR)/fragment_processor.cc

$(BUILD_DIR)/vertex_processor.o: $(SRC_DIR)/vertex_processor.cc
	g++ -I $(INCLUDE_DIR) -c -o $(BUILD_DIR)/vertex_processor.o $(SRC_DIR)/vertex_processor.cc

$(BUILD_DIR)/vao_vbo.o: $(SRC_DIR)/vao_vbo.cc
	g++ -I $(INCLUDE_DIR) -c -o $(BUILD_DIR)/vao_vbo.o $(SRC_DIR)/vao_vbo.cc

$(BUILD_DIR)/options.o: $(SRC_DIR)/vao_vbo.cc
	g++ -I $(INCLUDE_DIR) -c -o $(BUILD_DIR)/options.o $(SRC_DIR)/options.cc

.PHONY: clean
clean:
	-del $(OBJS) main.exe