cmake_minimum_required(VERSION 2.8)

PROJECT(Core)

ADD_DEFINITIONS(-fpermissive)

INCLUDE_DIRECTORIES(core_include)

FILE(GLOB MAIN 		src/*.cpp)
FILE(GLOB ADAPTER 	src/adapter/api_linux.cpp
					src/adapter/audio_linux.cpp
					src/adapter/msg_linux.cpp)

ADD_LIBRARY(core
			${MAIN}
			${ADAPTER})
TARGET_LINK_LIBRARIES(core 
			pthread
			dl
			sample)
