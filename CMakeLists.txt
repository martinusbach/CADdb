project(boost-experiment)
cmake_minimum_required(VERSION 2.8)

set(BOOST_ROOT "C:/temp/boost_1_60_0/boost_1_60_0")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories("C:/temp/boost_1_60_0/boost_1_60_0/stage/lib")

set(SRC_LIST
    main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST})

