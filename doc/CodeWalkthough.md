# Code intro
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
