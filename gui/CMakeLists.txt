cmake_minimum_required(VERSION 2.8)

PROJECT(Gui)

INCLUDE_DIRECTORIES(. ../core)

FILE(GLOB SRC 		src/*.cpp)

ADD_LIBRARY(gui ${SRC})

TARGET_LINK_LIBRARIES(gui core sample)
