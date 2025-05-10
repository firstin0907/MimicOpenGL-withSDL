cmake_minimum_required(VERSION 3.24)

project(MimicOpenGL)
add_definitions(-DSDL_MAIN_HANDLED)
set(SRC_FILES
    ./mimic/src/fragment_processor.cc
    ./mimic/src/mimic.cc
    ./mimic/src/options.cc
    ./mimic/src/per_sample_operator.cc
    ./mimic/src/scan_converser.cc
    ./mimic/src/vao_vbo.cc
    ./mimic/src/vertex_processor.cc
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_PREFIX_PATH "./SDL2-2.32.2/cmake")
find_package(SDL2 REQUIRED)

include_directories(main ${SDL2_INCLUDE_DIRS})

add_executable(main ${SRC_FILES} main.cc )
target_link_libraries(main ${SDL2_LIBRARIES})
