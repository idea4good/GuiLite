cmake_minimum_required(VERSION 2.8)

PROJECT(Gui)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/..)

INCLUDE_DIRECTORIES(../core
					gui_include)

FILE(GLOB SRC 		src/*.cpp)

ADD_LIBRARY(gui ${SRC})

TARGET_LINK_LIBRARIES(gui core sample)
