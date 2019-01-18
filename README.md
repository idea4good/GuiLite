# GuiLite - The samllest UI framework
- [中文](doc/README-cn.md)
- [Features](#Features)
- [Demo](#Demo)
- [Documents](#Documents)
- [Videos](#Videos)
- [Thanks](#Thanks)
***
## What is GuiLite?
- GuiLite has 5000 lines of C++ code, providing **UI framework for all platforms** for developers. Just like the pictures above, GuiLite can run on iOS, Android, GNU/Linux, Windows, macOS and **MCU** platforms.
- GuiLite is a embedded UI framework. It can work with Qt/MFC/UWP/XCode, which means you can get not only GuiLite features but also iOS/Android/macOS/Qt/MFC/UWP features.
- GuiLite can work with other languages(e.g. Swift, Java, C#).

## New feature: Wave demo on MCU -- HelloWave
![HelloWave](doc/HelloWave.gif)
- Hardware: STM32F103ZET6(512K ROM, 64K RAM) + 240*320 16bits color TFT LCD
- Software: None OS + GuiLite
- **Believe or not, it's only 100+ lines code!** [Check the code](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloWave/UIcode/UIcode.cpp). Showing you how to use wave widget, and port it on MCU.
- [How to porting for any MCU?](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloWave/README.md#How-to-port-on-any-MCU-)

## Features
### Cross platform
Ran on macOS & iOS:

![macOS](doc/Mac.gif) ![iOS](doc/Ios.landscape.gif)

Ran on Android & GNU/Linux embedded (ARM):

![Android](doc/Android.gif) ![GNU/Linux embedded (ARM)](doc/Linux.gif)

Ran on Windows Mixed Reality:

![Win MR](doc/WinMR.gif)

### Unicode & Bitmap
Wallpaper with bitmap:

![wallpaper](doc/wallpaper.jpg)

Multi language:

![unicode](doc/unicode.jpg)

[How to build unicode font/bitmap resource?](https://github.com/idea4good/GuiLiteToolkit)

### Multi theme
![multi-theme](doc/multi-theme.png)

[How to switch theme?](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/SampleCode/source/resource/resource.cpp)

### Self monitor
**Report build activities to cloud:**

![BuildInfo](doc/BuildInfo.png)

**Sync local data to cloud:**

![DataOnCloud](doc/data_on_cloud.png)

## Demo
GuiLite just a framework, it could not create UI by itself. In order to show how to build UI App with it, We provide some samples for your reference.
- Click [here](https://github.com/idea4good/GuiLiteSamples) to preview UI effect.
- Sample code discription:

| Sample name | Platform | Discription | Build code | level |
| --- | --- | --- | --- | --- |
| HelloGuiLite | Windows, Linux | Show how to Initilize UI, load resource, layout UI | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloGuiLite/README.md) | Beginner |
| HelloFont | Windows, Linux | Show how to use muti languages(UTF-8) | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloFont/README.md) | Beginner |
| HelloAnimation | Windows, Linux | Show how to make animation | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloAnimation/README.md) | Beginner |
| HelloSlide | Windows, Linux | Show how to use sliding pages | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloSlide/README.md) | Intermediate |
| HelloWave | Windows, STM32F103, STM32F429 | Show how to use wave widget, and port it on MCU | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloWave/README.md) | Intermediate |
| HostMonitor | iOS, Mac, Android, Windows, Linux | Show how to build complex UI for all platforms | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/README.md) | Master |

## Documents
- [How to build?](doc/HowToBuild.md)
- [How GuiLite work?](doc/CodeWalkthrough.md)
- [UML chart](doc/UML.md)
- [How to layout widgets?](doc/HowLayoutWork.md)
- [How to dispatch messages?](doc/HowMessageWork.md)

## Videos
- [GuiLite introduction](https://www.youtube.com/watch?v=grqXEz3bdC0)
- [3D GuiLite](https://v.youku.com/v_show/id_XMzYxNTE3MTI0MA)
- [GuiLite + STM32](https://v.youku.com/v_show/id_XNDAwNzM5MTM3Ng)

## Gitee link
[Link to Gitee in China](https://gitee.com/idea4good/GuiLite)

## Thanks
### Thanks the help from QQ group, you guys make GuiLite better! and welcome new frinend to join us.
![QQ group: 527251257](doc/qq.group.jpg)
### Thanks the donations from [Gitee users](https://gitee.com/idea4good/GuiLite), every penny of donations will be used for needy children or family of China.
![Donators](doc/donation.jpg)
