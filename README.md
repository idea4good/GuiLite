# GuiLite - made by 6000 lines code
[Chinese](README-cn.md)
GuiLite is written by 6000 lines C/C++ code, just like MFC/QT, it provide UI framework for developer.

GuiLite is a embeded UI framework, it can work with QT/MFC/UWP APP, this mean you can not only develop your UI with Guilite, and also get QT/MFC/UWP/Android UI feature as well.

We hope you can develop some stable feature with GuiLite, then you can run your APP on Linux/Android/Windows/VR/MR at same time.

Get some examples, you can visit [GuiLiteSamples](https://github.com/idea4good/GuiLiteSamples)

## Code intro
gui folder:
1. Here are code for controllers(eg. button, lable, keyboard) and containers(eg. view, dialog, flip pages); You can redraw your own widget by modify this code.
2. Here are code for message transition(eg. finger press/release), it will pass finger press/release message to widget which been touched, and call callback function if defined.

core folder:
1. Here are code for render.
2. Here are code for OS encapsulation.

## How GuiLite work?

## How manage widgets?

### How to create widgets?

### How to manage widgets?

## Render

### Render methods

### Graphic layer
![Graphic layer](GraphicLayer.png)
