cmake_minimum_required(VERSION 3.5.1)
project(ImageProcessing CXX)
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_compile_options("/W3")
else()
    add_compile_options("-Wall")
endif()

find_package(OpenCV REQUIRED)
include_directories(include ${OpenCV_INCLUDE_DIRS})

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES src/main.cpp src/noise.cpp)
set(HEADERS include/noise.h)

add_executable(ImageProcessing ${SOURCES} ${HEADERS})
target_link_libraries(ImageProcessing PUBLIC ${OpenCV_LIBS})
