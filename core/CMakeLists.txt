cmake_minimum_required(VERSION 2.8)

PROJECT(Core)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/..)

INCLUDE_DIRECTORIES(core_include)

FILE(GLOB MAIN 		src/*.cpp)
FILE(GLOB ADAPTER 	src/adapter/api_linux.cpp
					src/adapter/audio_linux.cpp
					src/adapter/msg_linux.cpp)

ADD_LIBRARY(core
			${MAIN}
			${ADAPTER})

execute_process(
    COMMAND ${CMAKE_SOURCE_DIR}/.sync.sh Core
)
