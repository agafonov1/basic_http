cmake_minimum_required(VERSION 2.8)

project(final)

set(Boost_USE_STATIC_LIBS OFF)

find_package(Boost COMPONENTS system thread REQUIRED)
if(Boost_FOUND)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)


set(SOURCE_EXE main.cpp)
set(SOURCE_LIB server.cpp)

add_library(server STATIC ${SOURCE_LIB})
add_executable(final ${SOURCE_EXE})

target_link_libraries (final server)
target_link_libraries (final ${Boost_SYSTEM_LIBRARY})
target_link_libraries (final ${Boost_LIBRARIES})
