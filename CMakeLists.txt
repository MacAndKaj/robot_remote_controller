cmake_minimum_required(VERSION 3.18)

project(ps5_reader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3")

find_package(Boost 1.86 COMPONENTS date_time log log_setup program_options system)

set(SOURCE_FILES 
    src/logging.hpp

    src/main.cpp
)

add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_SOURCE_DIR}/src
    PUBLIC ${Boost_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    ${Boost_LIBRARIES} 
    connection
    ps5_ctrl
)
