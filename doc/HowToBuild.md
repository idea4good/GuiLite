## How to build GuiLite libary for Windows?
Prerequisite: Windows 10, visul stdio 2015/2017
- Open "GuiLite.sln" by Visual studio 2017
- Click `Build Solution`
- Output here: \GuiLite\Debug(Release)\GuiLite.lib

## How to build GuiLite libary for iOS/Mac and Linux(amd64) & raspberry pi?
- `cd GuiLite`
- `cmake .`
- `make`
- Output here: /GuiLite/libGuiLite.a

## How to build GuiLite libary for ARM Linux?
### Prerequisite: Install cross compiler:
- For ARM32: `sudo apt-get install g++-arm-linux-gnueabi gcc-arm-linux-gnueabi`
- For ARM64: `sudo apt-get install g++-aarch64-linux-gnu gcc-aarch64-linux-gnu`
### Build
- `cd GuiLite`
- For ARM32: `cmake -D CMAKE_C_COMPILER="/usr/bin/arm-linux-gnueabi-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/arm-linux-gnueabi-g++" .`
- For ARM64: `cmake -D CMAKE_C_COMPILER="/usr/bin/aarch64-linux-gnu-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/aarch64-linux-gnu-g++" .`
- `make`
- Output here: /GuiLite/libGuiLite.a

## How to build GuiLite libary for Android?
### Prerequisite: Install cross compiler:
`sudo apt-get install gcc-arm-linux-androideabi` 
### Build
- `cd GuiLite`
- `cmake -D CMAKE_C_COMPILER="/usr/bin/arm-linux-androideabi-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/arm-linux-androideabi-g++" .`
- `make`
- Output here: /GuiLite/libGuiLite.a

## How to build GuiLite libary for any MCU?
### Prerequisite: Install Keil uvsion 5.6 or above.
### Build
- Open `GuiLite.uvprojx` with Keil uvsion.
- Implement function(e.g, thread_sleep) in `core\src\adapter\api_unknow.cpp` to meet your need.
- Choose your Device type(Default: STM32F103ZE) from option for target.
- Build GuiLite.
- Output here: /GuiLite/Objects/GuiLite.lib
