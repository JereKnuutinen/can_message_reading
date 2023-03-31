cmake_minimum_required(VERSION 3.11.0)

project(can_message_reading)

enable_language(CXX)

# use C++ standard version 17
set(CMAKE_CXX_STANDARD 17)
# produce non-optimized code with debug symbols
set(CMAKE_BUILD_TYPE Debug)


add_executable (can_cpp src/can_cpp.cpp)
target_link_libraries(can_cpp -lsocketcan)
target_include_directories(can_cpp PUBLIC .)
target_include_directories(can_cpp PUBLIC src)

