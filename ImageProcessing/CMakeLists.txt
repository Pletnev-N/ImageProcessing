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

set(SOURCES src/transforms.cpp src/noise.cpp src/filters.cpp src/utils.cpp)
set(HEADERS include/transforms.h include/noise.h include/filters.h include/utils.h)

add_executable(ImageProcessing src/main.cpp ${SOURCES} ${HEADERS})
target_link_libraries(ImageProcessing PUBLIC ${OpenCV_LIBS})

add_executable(procdir src/procdir.cpp ${SOURCES} ${HEADERS})
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_link_libraries(procdir PUBLIC ${OpenCV_LIBS} stdc++fs)
else()
  target_link_libraries(procdir PUBLIC ${OpenCV_LIBS})
endif()
