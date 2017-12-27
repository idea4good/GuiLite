# GuiLite - 揭示UI原理，打造你自己的UI（框架） - 开发者qq群：`527251257`
![Mixed Reality](WinMR.gif) ![Android](Android.gif)

超轻量UI框架 - GuiLite是由6千行左右的C/C++代码编写而成，它像MFC，QT一样为开发者提供界面支持。

- GuiLite可以完美运行在Android，Windows（包含VR），Mac，**单片机**和**市面所有的 ARM Linux物联网终端**设备上。
- GuiLite可以嵌入在MFC、QT、 Android等其他UI系统中。让你的界面集百家之长，又不失个性。
- GuiLite鼓励混合编程，开发者可以用C/C++编写的GuiLite接管UI部分，用Java，Go，C#，Python发开业务部分。

相比其他强大的UI框架，GuiLite只聚焦界面开发（特别是手机风格的滑动界面），只使用最基础的C++特性，直达绘制底层。希望通过减少代码，调用层次，让UI框架的体积最小，效率更高，使之流畅运行在：手机，电脑，单片机等硬件环境上。

## 为什么开发GuiLite？如何使用？
任何UI框架都不是为你我而生的，只有掌握核心原理，对其深度定制，才能真正掌握自己的命运！因此，我们开发了GuiLite，希望用这6千行代码揭示UI的核心原理和定制方法。

我们不推荐开发者全盘使用GuiLite；鼓励开发者在吃透代码后，根据自身业务特点，修改出更加丰富、个性的界面；或者发展出适合自己的UI框架。

为了聚焦UI核心原理，GuiLite会一直保持单片机的代码体量。

## 快速上手
- 下载实例代码: `git clone https://github.com/idea4good/GuiLiteSamples.git`
- `cd GuiLiteSamples\HostMonitor`
- [编译/运行在任意平台上](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/README.md)

## 开发文档
[代码走读](CodeWalkthough-cn.md)

[如何编译?](HowToBuild.md)

[如何布局UI?](HowLayoutWork.md)

[如何传递消息?](HowMessageWork.md)

## 实例代码及运行效果:
[GuiLiteSamples repository](https://github.com/idea4good/GuiLiteSamples)

[GuiLiteSamples video](http://v.youku.com/v_show/id_XMzA5NTMzMTYyOA)
