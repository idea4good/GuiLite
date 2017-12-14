## How to build core/gui libary for Windows?
depdency: Windows 10, visul stdio 2015/2017
- Open "GuiLite.sln" by Visual studio 2017
- Click `Build Solution`
- You get \GuiLite\Debug(Release)\core.lib gui.lib

## How to build core/gui libary for Linux（x64 & raspberry pi）?
- `cd GuiLite`
- `cmake .`
- `make`
- You get \GuiLite\libcore.a libgui.a

## How to build core/gui libary for ARM Linux?
- `cd GuiLite`
- `cmake -D CMAKE_C_COMPILER="/usr/bin/arm-linux-gnueabi-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/arm-linux-gnueabi-g++" .`
- `make`
- You get \GuiLite\libcore.a libgui.a

## How to build core/gui libary for Android?
- `cd GuiLite`
- `sudo apt-get install gcc-arm-linux-androideabi` skip this if you have got the tool
- `cmake cmake -D CMAKE_C_COMPILER="/usr/bin/arm-linux-androideabi-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/arm-linux-androideabi-g++" .`
- `make`
- You get \GuiLite\libcore.a libgui.a
