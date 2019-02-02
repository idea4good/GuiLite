# GuiLite - The smallest UI framework
- [中文](doc/README-cn.md)
- [Features](#Features)
- [Demo](#Demo)
- [Documents](#Documents)
- [How to Learn?](#How-to-Learn)
- [Videos](#Videos)
- [Thanks](#Thanks)
***
## What is GuiLite?
- GuiLite, a tiny and light framework, has only 5000 lines of C++ code, supporting **all computing platforms**. It could run on iOS, Android, GNU/Linux, Windows, macOS and **all ARM Linux** IOT platforms.
- GuiLite is without reference to OS, it even could run on any kind of **MCU without OS**.
- GuiLite is a embedded framework. It can work with another UI framework such as Qt/MFC/UWP/XCode, which means you can get not only GuiLite features but also iOS/Android/macOS/Qt/MFC/UWP features on your project.
- GuiLite can work with other languages(e.g. Swift, Java, C#).
- ⚠️We never encourage people accept GuiLite totally and uncritically, but want people master principles of UI itself.
- ⚙️We hope GuiLite is small enough to help you understand how UI work, and build unique UI yourself.
- 👑You should be the master of your UI, not any UI library(include GuiLite).

## New feature: Demo on MCU -- HelloWave + HelloParticle
![HelloWave](doc/HelloWave.gif)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;![HelloParticle](doc/HelloParticle.gif)
- MCU Hardware: STM32F103ZET6(512K ROM, 64K RAM) + 240*320 16bits color TFT LCD
- MCU Software: None OS + GuiLite
- **Hello Wave has only 100+ lines code!** [Check the code](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloWave/UIcode/UIcode.cpp). Showing you how to use wave widget.
- **Hello Particle has only 100- lines code!** [Check the code](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloParticle/UIcode/UIcode.cpp). Showing you how to develop particle system.
- [How to porting on any MCU in 5 minutes?](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloWave/README.md#How-to-port-on-any-MCU-)
- Also support: Windows & Linux

## Features
### Cross platform
On macOS & iOS:

![macOS](doc/Mac.gif) ![iOS](doc/Ios.landscape.gif)

On Android & GNU/Linux embedded (ARM):

![Android](doc/Android.gif) ![GNU/Linux embedded (ARM)](doc/Linux.gif)

On Windows Mixed Reality & MCU:

![Win MR](doc/WinMR.gif) ![MCU](doc/MCU.gif)

### Unicode & Bitmap
Wallpaper with bitmap:

![wallpaper](doc/wallpaper.jpg)

### Multi-language:

![unicode](doc/unicode.jpg)

[How to build unicode font/bitmap resource?](https://github.com/idea4good/GuiLiteToolkit)

### Multi-theme
![multi-theme](doc/multi-theme.png)

[How to switch theme?](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/SampleCode/source/resource/resource.cpp)

### Self monitor
**Report build activities to cloud:**

![BuildInfo](doc/BuildInfo.png)

**Sync local data to cloud:**

![DataOnCloud](doc/data_on_cloud.png)

## Demo
GuiLite is only a framework, it could not create UI by itself. In order to show how to build UI App with it, We provide some samples for you.
- Click [here](https://github.com/idea4good/GuiLiteSamples) to preview its effect.
- Sample code description:

| Sample name | Platform | Description | Build code | level |
| --- | --- | --- | --- | --- |
| HelloGuiLite | Windows, Linux | Show how to Initialize UI, load resource, layout UI | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloGuiLite/README.md) | ★★ |
| HelloFont | Windows, Linux | Show how to use multi-language feature(UTF-8) | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloFont/README.md) | ★★ |
| HelloAnimation | Windows, Linux | Show how to make animation | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloAnimation/README.md) | ★★ |
| HelloParticle | Windows, Linux, STM32F103, STM32F429 | Show how to develop particle system | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloParticle/README.md) | ★ |
| HelloSlide | Windows, Linux | Show how to use sliding pages | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloSlide/README.md) | ★★★ |
| HelloWave | Windows, Linux, STM32F103, STM32F429 | Show how to use wave widget | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloWave/README.md) | ★★★ |
| HostMonitor | iOS, Mac, Android, Windows, Linux | Show how to build complex UI for all platforms | [Build](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/README.md) | ★★★★ |

## Documents
- [How to build?](doc/HowToBuild.md)
- [How GuiLite work?](doc/CodeWalkthrough.md)
- [UML chart](doc/UML.md)
- [Layout widgets?](doc/HowLayoutWork.md)
- [How to dispatch messages?](doc/HowMessageWork.md)

## How to learn?
1. Build/Run GuiLite library
2. Build/Run HelloXXX demos
3. Read/Modify `HelloXXX/UIcode/UIcode.cpp` code
4. Read/Modify `gui` code
5. Read/Modify `core` code
6. Build your UI framework

## Videos
- [GuiLite introduction](https://www.youtube.com/watch?v=grqXEz3bdC0)
- [3D GuiLite](https://v.youku.com/v_show/id_XMzYxNTE3MTI0MA)
- [GuiLite + STM32](https://v.youku.com/v_show/id_XNDAwNzM5MTM3Ng)

## Gitee link
[Gitee in China](https://gitee.com/idea4good/GuiLite)

## Thanks
### Thanks the help from QQ group, you guys make GuiLite better! And welcome new friend to join us.
![QQ group: 527251257](doc/qq.group.jpg)
### Thanks the donations from [Gitee users](https://gitee.com/idea4good/GuiLite), every penny of donations will be used for needy children or family of China.
![Donators](doc/donation.jpg)

