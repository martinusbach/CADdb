project(CadDB)
cmake_minimum_required(VERSION 2.8)

# Boost libraries
set(BOOST_ROOT "C:/boost/boost_1_61_0")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
set (Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS filesystem system program_options log log_setup chrono atomic date_time thread REQUIRED)

# SQLite3 library
set(SQLITE3_HEADERS_DIR "ext_libs/sqlite3")
add_library(sqlite3 STATIC "ext_libs/sqlite3/sqlite3.c")

# CadDB sources
set(SRC_CadDB
    main.cpp
    my_logger.cpp
    caddb.cpp
)

# to supress MSVS warnings about safe functions
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# define DEBUG for debug builds
add_compile_options( $<$<CONFIG:Debug>:-DDEBUG> )

# work around some bugs in Clang
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /O1 /Ob1 /D NDEBUG")  # needed because of a bug in clang
    add_compile_options( -DBOOST_NO_CXX11_NUMERIC_LIMITS ) # needed because of a bug in libc++ of clang, see http://stackoverflow.com/questions/14300136
endif()


# main executable
set(MAIN_TARGET_NAME ${PROJECT_NAME})
add_executable(${MAIN_TARGET_NAME} ${SRC_CadDB})
target_link_libraries(${MAIN_TARGET_NAME}
    sqlite3
    ${Boost_LIBRARIES}
)
target_include_directories(${MAIN_TARGET_NAME} PUBLIC
    ${SQLITE3_HEADERS_DIR}
    ${Boost_INCLUDE_DIRS}
)
