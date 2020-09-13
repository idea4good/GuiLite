# How to use GuiLite?
- For GuiLite user: You could copy GuiLite.h in your application
- For GuiLite developer: You could get well-organized source code, and develop it on any platform(Windows/Linux/Apple)

# For GuiLite user
GuiLite is a header-only library, so it should be straightforward to integrate into your application.

UIcode.cpp:
```c++
#define GUILITE_ON	// Do not define this macro upon GuiLite.h once more
#include "GuiLite.h"

// your code here:
```
# For GuiLite developer
## Build GuiLite libary for Windows?
Prerequisite: Windows & Visul Studio 2013/2015/2017/2019
- Open "GuiLite.sln" by Visual Studio
- Click `Build Solution`
- Output here: GuiLite\workspace\Debug(Release)\GuiLite.lib

FAQ: Error when open GuiLite project with Visual Studio, reconfigure the project to match your computer like this:

![vs-configure](vs-configure.jpg)

## Build GuiLite libary for iOS/Mac and Linux(amd64) & raspberry pi?
```bash
cd GuiLite/workspace
cmake .
make

# Output here: GuiLite/workspace/libGuiLite.a
```

## Build GuiLite libary for ARM Linux?
### Prerequisite: Install cross compiler:
```bash
# For ARM32:
sudo apt-get install g++-arm-linux-gnueabi gcc-arm-linux-gnueabi
# For ARM64:
sudo apt-get install g++-aarch64-linux-gnu gcc-aarch64-linux-gnu
```
### Build
```bash
cd GuiLite/workspace
# For ARM32:
cmake -D CMAKE_C_COMPILER="/usr/bin/arm-linux-gnueabi-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/arm-linux-gnueabi-g++" .
# For ARM64:
cmake -D CMAKE_C_COMPILER="/usr/bin/aarch64-linux-gnu-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/aarch64-linux-gnu-g++" .
make
```
Output here: GuiLite/workspace/libGuiLite.a

## How to build GuiLite libary for any MCU?
### Prerequisite: Install Keil uvsion 5.6 or above.
### Build
- Open `GuiLite/workspace/GuiLite.uvprojx` with Keil uvsion.
- Implement function(e.g, thread_sleep) in `core\src\adapter\api_unknow.cpp` to meet your need.
- Choose your Device type(Default: STM32F103ZE) from option for target.
- Build GuiLite.
- Output here: GuiLite/workspace/Objects/GuiLite.lib

## Build header-only GuiLite.h?
```bash
cd GuiLite/workspace
./header-only.sh
```
