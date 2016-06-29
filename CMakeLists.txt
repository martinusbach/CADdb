project(boost-experiment)
cmake_minimum_required(VERSION 2.8)

# Boost libraries
set(BOOST_ROOT "C:/boost/1.60.0")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories("${BOOST_ROOT}/stage/lib")

#SQLite3 library
include_directories("ext_libs/sqlite3")
set(SQLITE3_C "ext_libs/sqlite3/sqlite3.c")


set(SRC_LIST
    main.cpp
    my_logger.cpp
    caddb.cpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${SQLITE3_C})

