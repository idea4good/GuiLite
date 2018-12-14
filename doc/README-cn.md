# GuiLite - 简洁出奇迹
- [功能介绍](#功能介绍)
- [实例程序](#实例程序)
- [开发文档](#开发文档)
- [致谢](#致谢)
***
## GuiLite是什么鬼？
- GuiLite（超轻量UI框架）是5千行代码的**全平台UI框架**，可以完美运行在iOS，Android，Windows（包含VR），Mac，单片机和**市面所有的 ARM Linux物联网终端设备**上。
- GuiLite可以嵌入在iOS、Android、MFC、QT等其他UI系统中，让你的界面集百家之长，又不失个性。
- GuiLite鼓励混合编程，开发者可以用GuiLite接管UI部分，用Swift，Java，Go，C#，Python开发业务部分。

相比QT、MFC，GuiLite不预设开发者的使用场景，不在具体功能上，大包大揽；用框架的简洁，换取开发的自由；GuiLite在图形绘制上面，力图一步到位，运行效率感人。

## 为什么开发GuiLite？如何使用？
- 任何UI框架都不是为你我而生的，只有掌握核心原理，对其深度定制，才能真正掌握自己的命运！因此，我们开发了GuiLite，希望用这5千行代码揭示UI的核心原理和定制方法。
- 我们不推荐开发者全盘使用GuiLite；鼓励开发者在吃透代码后，根据自身业务特点，修改出更加丰富、个性的界面；或者发展出适合自己的UI框架。
- 为了聚焦UI核心原理，GuiLite会一直保持单片机的代码体量。

## 新功能：多语言demo -- HelloFont
![HelloFont](HelloFont.gif)

**信不信由你，代码只有100+行！**[查阅代码](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloFont/UIcode/UIcode.cpp)

## 功能介绍
### 卓越的跨平台能力
在Mac, iOS下的运行效果:

![Mac](Mac.gif) ![iOS](Ios.landscape.gif)

在Android，嵌入式ARM Linux下的运行效果:

![Android](Android.gif) ![Linux](Linux.gif)

在Windows混合现实下的运行效果:

![Win MR](WinMR.gif)

### 万国语和墙纸
墙纸：
![unicode](wallpaper.jpg)

万国语(unicode)：
![unicode](unicode.jpg)

[如何制作多种文字/位图资源?](https://github.com/idea4good/GuiLiteToolkit)

### “换肤”功能
![multi-theme](multi-theme.png)

[如何“换肤”？](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/SampleCode/source/resource/resource.cpp)

### 自我监控
**上报“编译/运行”情况至“云端”：**

![BuildInfo](BuildInfo.png)

**同步“本地数据”至“云端”：**

![DataOnCloud](data_on_cloud.png)

## 实例程序
GuiLite只是一个框架，本身并不能生成UI。为了能够展示如何用GuiLite开发App，我们提供了一些实例程序给大家参考。
- 点击[这里](https://github.com/idea4good/GuiLiteSamples)预览UI效果。
- 实例程序的功能说明：

| 实例名称 | 支持的平台 | 功能简介 | 编译方法 | 难度 |
| --- | --- | --- | --- | --- |
| HelloGuiLite | Windows, Linux | 初始化GuiLite，加载资源，布局界面元素，按钮响应 | [编译/运行](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/README.md) | 初级 |
| HelloFont | Windows, Linux | 显示多种语言（不限：中、英文） | [编译/运行](https://github.com/idea4good/GuiLiteSamples/blob/master/HostFont/README.md) | 初级 |
| HelloSlide | Windows, Linux | 滑屏界面的应用 | [编译/运行](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloSlide/README.md) | 中级 |
| HostMonitor | iOS, Mac, Android, Windows, Linux | 创建复杂界面，扩展自定义控件，适配全平台 | [编译/运行](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/README.md) | 高级 |
## 开发文档
- [设计原理](CodeWalkthrough-cn.md)
- [UML示意图](UML.md)
- [如何布局UI?](HowLayoutWork.md)
- [如何传递消息?](HowMessageWork.md)

## 视频链接:
- [GuiLite简介](https://v.youku.com/v_show/id_XMzA5NTMzMTYyOA)
- [3D GuiLite](https://v.youku.com/v_show/id_XMzYxNTE3MTI0MA)

## Gitee链接
[国内码云链接](https://gitee.com/idea4good/GuiLite)

## 致谢
### 感谢开发者群的所有同学，是你们塑造了今天的GuiLite！也欢迎新的大神/小白加入我们。
![QQ group: 527251257](qq.group.jpg)
### 感谢来自[码云用户](https://gitee.com/idea4good/GuiLite)的捐赠，您的每一分钱都会用来帮助急需帮助的中国儿童或家庭。
![Donators](donation.jpg)
