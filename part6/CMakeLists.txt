cmake_minimum_required(VERSION 3.16)

project(GKD_Part6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenCV_DIR
    "C:/Users/admin/opencv-work/opencv/install/x86/mingw/lib"
)

find_package(OpenCV REQUIRED)

add_executable(part6
    main.cpp
    matrix.cpp
)

add_executable(server
    server.cpp
    matrix.cpp
)

target_link_libraries(part6 PRIVATE
    ${OpenCV_LIBS}
    ws2_32
)

target_link_libraries(server PRIVATE
    ws2_32
)