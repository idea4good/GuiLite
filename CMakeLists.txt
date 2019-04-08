cmake_minimum_required(VERSION 2.8)

PROJECT(GuiLite)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

INCLUDE_DIRECTORIES(core widgets)

# core
FILE(GLOB CORE_SRC      core/src/*.cpp)
FILE(GLOB CORE_ADAPTER  core/src/adapter/api_linux.cpp
                        core/src/adapter/audio_linux.cpp
                        core/src/adapter/msg_linux.cpp)
# gui
FILE(GLOB WIDGETS_SRC       widgets/src/*.cpp)

# build static library
ADD_LIBRARY(GuiLite STATIC ${CORE_SRC} ${CORE_ADAPTER} ${WIDGETS_SRC})

# build shared library
# ADD_LIBRARY(GuiLite SHARED ${CORE_SRC} ${CORE_ADAPTER} ${WIDGETS_SRC})

execute_process(COMMAND ${CMAKE_SOURCE_DIR}/.sync.sh GuiLite)
