# GuiLite - made by 6000 lines code
[中文](README-cn.md)

GuiLite is written by 6000 lines C/C++ code, just like MFC/QT, it provide UI framework for developer.

GuiLite is a embeded UI framework, it can work with QT/MFC/UWP APP, this mean you can not only develop your UI with Guilite, and also get QT/MFC/UWP/Android UI feature as well.

We hope you can develop some stable feature with GuiLite, then you can run your APP on Linux/Android/Windows/VR/MR at same time.

Get some examples, you can visit:

[GuiLiteSamples repository](https://github.com/idea4good/GuiLiteSamples)

[GuiLiteSamples video](https://www.youtube.com/watch?v=grqXEz3bdC0)

## Code intro
gui folder:
1. Here are code for controllers(eg. button, lable, keyboard) and containers(eg. view, dialog, flip pages); You can redraw your own widget by modify this code.
2. Here are code for message transition(eg. finger press/release), it will pass finger press/release message to widget which been touched, and call callback function if defined.

core folder:
1. Here are code for render: surface.cpp, bitmap.cpp, word.cpp
2. Here are code for OS encapsulation: api_linux.cpp api_win.cpp

## How GuiLite work?
GuiLite do 2 things: one is widgets management, the other is render.

### How to manage widgets?
All widgets derived from class c_wnd. The widget will be created when the class has been instanced, at this time, the widget still alone, has nothing to do with UI system, could not response any user operation. We need use "connect()" function to link this widget into UI system. use "disconnect()" function to unlink this widget from UI system.

### How to render?
See function draw_xxx() in surface.cpp word.cpp bitmap.cpp.

### Graphic layer
![Graphic layer](GraphicLayer.png)

display layer：
This layer for physical display device, one UI has only one display layer.

surface layer:
One display layer have many surface layers, one surface layer represent one flip page.

frame layer:
One surface layer has many frame layers, one frame layer represent one layer in Z direction.

