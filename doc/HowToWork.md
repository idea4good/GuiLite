# How GuiLite work
- [What GuiLite do?](#What-GuiLite-do-)
- [How to customize/extend GuiLite?](#How-to-customize/extend-GuiLite-)
- [Code intro](#Code-intro)
- [Widgets management](#Widgets-management)
- [Rendering](#Rendering)
***
## What GuiLite do?
GuiLite do 2 things only: widgets management and rendering.

Widgets management include:
- Add/Remove GUI element(e.g, button, label, dialog), set caption and location.
- Dispatch/Response intput message: By analysis the location information from the message, GuiLite would find the widget which should be reponse, and call response function(e.g, on_mouse_click, on_keyboard_click)
- Dispatch/Response customer message: By analysis the ID of message, GuiLite would find response funciotn, and calll it(: on_timer, on_custmer_refresh)

Rendring include:
- Draw pixel, line, rect
- Set frame layer; While your drawing, you should input frame layer index to make your drawing at a specific layer
- Update frame layer; While frame layer changed(e.g, open/close dialog), GuiLite would determine which pixel of frame layer should be display on screen

👉Note: Rendering do not depend on Widgets management, In some case, you would render line/text/bitmap directly without any widgets in your GUI system. For example: Limted resource MCU platform.

## How to customize/extend GuiLite?
In order to make GuiLite simple and clear, GuiLite only provide basic widgets(e.g, button, label, keyboard, spinx etc) implementation. If you want do more things on widgets, you can do it like this:
- Update widgets code in GuiLite directly
- Add new widgets file, and implement new widgets by reference GuiLite widgets code.

If you would extend drawing method(e.g, draw circle/eclipse etc.), you can add your method in surface.cpp directly.

How to render?
See function draw_xxx() in surface.cpp word.cpp bitmap.cpp.

## Code intro
core folder:
- Implement message dispatch/response, multi frame layer management and rendering for all platforms
- Adapter GuiLite on Windows/Linux/Android/iOS/macOS or none OS in adapter folder

widgets folder:
- Here are code for controllers(eg. button, lable, keyboard) and containers(eg. view, dialog, flip pages); You can redraw your own widget by modify this code.
- Here are code for message transition(eg. finger press/release), it will pass finger press/release message to widget which been touched, and call callback function if defined.

## Widgets management
Widgets include button, spinbox, label, keyboard, dialog, view; GuiLite will link all widgets by function: connect, then GuiLite will easy to find/add/remove any widget quickly. All the connections looks like a tree.

For example: When you click a button, device get your finger location(x, y) will be send the root of the tree, GuiLite would find the button you click by comparing location information, then call the response function(e.g, repaint the button/change button status)

### How to create widgets?
All widgets derived from class c_wnd. The widget will be created when the class has been instanced, at this time, the widget still alone, has nothing to do with UI system, could not response any user operation.

### How to link/unlink widgets
By using "connect()" function, we can link this widget into widgets tree, then the widgets could response user touch/keyboard action.

By using "disconnect()" function to unlink this widget from UI system. then the widget still live in memory, but could not response user touch/keyboard action any more.

## Rendering
Rendering include rendering method and graphic management.
- All rendering method base on draw_pixel() function
- GuiLite support 3 frame layers so far, could handle 3 layers overlap senarios

### Rendering method
- Basic rendering function: surface_cpp.cpp::draw_xxx()
- Bitmap rendering function: bitmap.cpp::draw_bitmap_xxx()
- For take GPU advantage, you can refactor draw_xxx() by using GPU feature

### Graphic layer
![Graphic layer](GraphicLayer.png)
- display layer：This layer for physical display device, one UI has only one display layer
- surface layer: One display layer have many surface layers, one surface layer represent one flip page
- frame layer: One surface layer has many frame layers, one frame layer represent one layer in Z direction

## Source file annotation
| core Importance/File name | Introduction | Time cost |
| --- | --- | --- |
| ★★★ wnd.cpp | Basic class for widgets; connect/disconnect widgets; response user on_touch, on_key action | 1.5 hour |
| ★★ cmd_target.cpp | map/dispatch GUI/user custom message | 0.5 hour |
| ★★ display.cpp | Initialize display device, create surface,  | 0.5 hour |
| ★★ surface.cpp| Draw piexel/line/rect, and multi-layer management | 0.5 hour |
| word.cpp | Draw text | < 1 hour |
| bitmap.cpp | Draw bitmap | < 0.5 hour |
| rect.cpp | Rect for basic use(e.g, widgets location) | < 0.5 hour |
| api_linux.cpp | Adapter for Linux | < 0.5 hour |
| api_win.cpp | Adapter for Windows | < 0.5 hour |
| api_unknow.cpp | Adapter for other OS or none OS | < 0.5 hour |
| audio_linux.cpp | Adapter for Linux audio | < 0.5 hour |
| audio_win.cpp | Adapter Windows audio | < 0.5 hour |
| msg_linux.cpp | Implement message pipe on Linux | < 0.5 hour |
| msg_win.cpp | Implement message pipe on Windows | < 0.5 hour |
| msg_unknow.cpp | Implement message pipe on other OS or none OS | < 0.5 hour |
***
| widgets Difficulty/File name | Introduction | Time cost |
| --- | --- | --- |
| label.cpp | Source code for label | < 0.5 hour |
| button.cpp | Source code for button | < 0.5 hour |
| table.cpp | Source code for table | < 0.5 hour |
| dialog.cpp | Source code for dialog | < 0.5 hour |
| ★ gesture.cpp | Recognize finger gesture(e.g, press/release/slip) | 0.5 hour |
| ★ keyboard.cpp | Source code for keyboard | 0.5 hour |
| ★★ list_box.cpp | Source code for list box | 1.5 hour |
| ★★ spinbox.cpp | Source code for spin box | 1.5 hour |
| ★★ edit.cpp | Source code for edit widget | 1.5 hour |
| ★★★ wave_buffer.cpp | Source code for wave buffer | 1.5 hour |
| ★★★ wave_ctrl.cpp | Source code for wave widget | 1.5 hour |
