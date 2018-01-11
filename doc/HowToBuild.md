## How to build core&gui libary for Windows?
depdency: Windows 10, visul stdio 2015/2017
- Open "GuiLite.sln" by Visual studio 2017
- Click `Build Solution`
- Output here: \GuiLite\Debug(Release)\core.lib gui.lib

## How to build core&gui libary for Mac and Linux（x64 & raspberry pi）?
- `cd GuiLite`
- `cmake .`
- `make`
- Output here: /GuiLite/libcore.a libgui.a

## How to build core&gui libary for ARM Linux?
### Install cross compiler:
For example: arm-linux-gnueabi-gcc

`sudo apt-get install g++-arm-linux-gnueabi gcc-arm-linux-gnueabi`

- `cd GuiLite`
- `cmake -D CMAKE_C_COMPILER="/usr/bin/arm-linux-gnueabi-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/arm-linux-gnueabi-g++" .`
- `make`
- Output here: /GuiLite/libcore.a libgui.a

## How to build core&gui libary for Android?
### Install cross compiler:
`sudo apt-get install gcc-arm-linux-androideabi` 

- `cd GuiLite`
- `cmake -D CMAKE_C_COMPILER="/usr/bin/arm-linux-androideabi-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/arm-linux-androideabi-g++" .`
- `make`
- Output here: /GuiLite/libcore.a libgui.a
