# Welcome to GUI-lite
<p align="center">
    <img src="documents/logo.png" alt="Logo"/>
</p>
<p align="center">
  <img src="https://img.shields.io/badge/build-passing-brightgreen.svg">
  <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS%20%7C%20iOS%20%7C%20Android%20%7C%20MCU-brightgreen.svg">
  <img src="https://img.shields.io/badge/architecture-amd64%20%7C%20arm%20%7C%20arm64-blue.svg">
  <img src="https://img.shields.io/badge/license-Apache%202-blue.svg">
</p>
<p align="center">The smallest header-only GUI library (4 KLOC) for all platforms.</p>
<p align="center">
    <img src="documents/WhyGuiLite.png" alt="Why GuiLite"/>
</p>

- [中文](README_zh.md)

## Lightweight
- ✂️Small: 4,000+ lines of C++ code, zero dependency, header-only(GuiLite.h)
- ⚡Fast: High Rendering performance, even work on MCU
- 👫🏻Compatible: Work smoothly with 3rd party frameworks(Qt/MFC/Winform/Cocoa/Web)
- ⚙️️Hardware Minimum Requirements:

    | Processor | Disk/ROM space | Memory |
    | --- | --- | --- |
    | 24 MHZ | 29 KB | 9 KB |

## Cross platform
- Supported OSes: iOS/macOS/WatchOS, Android, Linux, Windows, RTOS... or **MCU without OS**
- Supported languages: C/C++, Swift, Java, Javascript, C#, Golang...
- Supported 3rd party libraries: Qt, MFC, Winforms, CoCoa...

## Hero features
- ☁️Cloud + IoT Solution: master your IoT business all over the world
- 🔣Multi-language, supports UTF-8;📀Playback Video
- 🔨[Toolkit](https://github.com/idea4good/GuiLiteToolkit) for building font/image resources
- 📐[Layout GUI WYSIWYG](https://github.com/idea4good/GuiLitePreviewer)
- 📊Code Telemetry and Analysis in real time(**remove .sh/.bat files if dislike to share**)
- 📦Supports 3D and [Web](https://github.com/idea4good/GuiLiteWeb)
- 🐋Run in docker with a single command: `sudo docker run -it --privileged -v /dev:/dev-share idea4good/gui-lite:latest bash /run.sh`

## Easy to learn and support
Even a C beginner could master GUI-lite quickly. The source code only uses basic C++ features (class, virtual function). We chose C++ as it could make the code size significantly smaller and easier to read.
- 📚Documentation
    - [How to use GUI-lite?](documents/HowToUse.md)
    - [Design specification](documents/HowToWork.md)
    - [How to Layout widgets?](documents/HowLayoutWork.md)
    - [How to build unicode font/bitmap resource?](https://github.com/idea4good/GuiLiteToolkit)
    - [How to switch theme?](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/UIcode/source/resource/resource.cpp)
    - [How to dispatch messages?](documents/HowMessageWork.md)
    - [UML chart of GUI-lite core](documents/UML.md)
- 📈Learning steps
    1. Build GUI-lite library
    2. Build/Debug HelloXXX demos
    3. Read/Modify `HelloXXX/UIcode/UIcode.cpp` code
    4. Read/Modify `widgets` code
    5. Read/Modify `core` code
    6. Build your GUI framework
- 📞[Reach out us](#community-channel) if you have any questions you are welcomed to our developer family.
- 🀄️[Mirror repository in China](https://gitee.com/idea4good/GuiLite)

## Demo wall
- Zero dependency, 100% build pass & runnable
- Click the demo you like, and run it on your hardware
- Most of the demos have about 100 lines of UI code, code repository is [here.](https://github.com/idea4good/GuiLiteSamples)
---
### MCU platform
---
<table>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/Hello3D"><img src="documents/Hello3D.gif" width="200px;"/><br /><sub><b>3D on STM32</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloWave"><img src="documents/HelloWave.gif" width="200px;"/><br /><sub><b>Wave on STM32</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloParticle"><img src="documents/HelloParticle.gif" width="200px;"/><br /><sub><b>Particle on STM32</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloStar"><img src="documents/HelloStar.gif" width="200px;"/><br /><sub><b>Star on STM32</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/Hello3Dwave"><img src="documents/Hello3Dwave.gif" width="200px;"/><br /><sub><b>3D wave on STM32</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloNoTouch"><img src="documents/HelloNoTouch.gif" width="200px;"/><br /><sub><b>Keyboard on STM32</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloMario"><img src="documents/HelloMario.gif" width="200px;"/><br /><sub><b>Mario on STM32</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloCircle"><img src="documents/HelloCircle.gif" width="200px;"/><br /><sub><b>3D circle on STM32</b></sub></a>
  </tr>
  <tr>
  <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/Hello3Ddonut"><img src="documents/Hello3Ddonut.gif" width="200px;"/><br /><sub><b>3D donut on STM32</b></sub></a>
  <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloTimer"><img src="documents/HelloTimer.gif" width="200px;"/><br /><sub><b>Timer</b></sub></a>
  <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloMolecule"><img src="documents/HelloMolecule.gif" width="200px;"/><br /><sub><b>Molecule move</b></sub></a>
  <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloPendulum"><img src="documents/HelloPendulum.gif" width="200px;"/><br /><sub><b>Pendulum effect</b></sub></a>
  </tr>
</table>

---
### IoT feature
---
<table>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/data_on_cloud.png" width="600px;"/><br /><sub><b>Monitor IoT device on cloud</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/BuildInfo.png" width="600px;"/><br /><sub><b>Code Telemetry & Analysis in real time</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloAzureIoT"><img src="documents/HelloAzureIoT.jpg" width="600px;"/><br /><sub><b>Track IoT device over the world</b></sub></a>
  </tr>
</table>

---
### Multi-language, Design tool and video
---
<table>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloFont"><img src="documents/HelloFont.gif" width="600px;"/><br /><sub><b>Lattice Font</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloFreetype"><img src="documents/HelloFreetype.gif" width="600px;"/><br /><sub><b>Freetype Font</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLitePreviewer"><img src="documents/Previewer.gif" width="600px;"/><br /><sub><b>Layout GUI</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloJPG"><img src="documents/HelloJPG.gif" width="600px;"/><br /><sub><b>Render JPG file quickly</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloFFmpeg"><img src="documents/HelloFFmpeg.jpg" width="600px;"/><br /><sub><b>Play video with FFmpeg</b></sub></a>
  </tr>
</table>

---
### Widgets & Controller
---
<table>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloGuiLite"><img src="documents/HelloGuiLite.gif" width="600px;"/><br /><sub><b>Emulate  Windows UI</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloScroll"><img src="documents/HelloScroll.gif" width="600px;"/><br /><sub><b>Scroll widget</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloWidgets"><img src="documents/HelloWidgets.gif" width="600px;"/><br /><sub><b>How to use widgets</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloNets"><img src="documents/HelloNets-pc.gif" width="600px;"/><br /><sub><b>3D Nets on Windows/Linux</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloTransparent"><img src="documents/HelloTransparent.gif" width="600px;"/><br /><sub><b>Transparent dialog</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloSlide"><img src="documents/HelloSlide.gif" width="600px;"/><br /><sub><b>Swipe view</b></sub></a>
  </tr>
</table>

---
### Cross platform
---
<table>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/HostMonitor.gif" width="300px;"/><br /><sub><b>HostMonitor on Windows</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/Mac.gif" width="300px;"/><br /><sub><b>HostMonitor on Mac</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/Android.gif" width="300px;"/><br /><sub><b>HostMonitor on Android</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/WinMR.gif" width="300px;"/><br /><sub><b>HostMonitor on Windows Mixed Reality</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/Linux.gif" width="300px;"/><br /><sub><b>HostMonitor on Linux</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteWeb"><img src="documents/GuiLite3D.gif" width="300px;"/><br /><sub><b>3D on Web</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteWeb"><img src="documents/GuiLiteCube.gif" width="300px;"/><br /><sub><b>3D on Web</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/iOS.gif" width="300px;"/><br /><sub><b>HostMonitor on iPhone</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteWeb"><img src="documents/GuiLiteGraphic.gif" width="300px;"/><br /><sub><b>How display work</b></sub></a>
  </tr>
</table>

---
### Apple platform
---
<table>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/Hello3D"><img src="documents/iWatch.hello3D.gif" width="300px;"/><br /><sub><b>3D on Apple Watch</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloWave"><img src="documents/iWatch.helloWave.gif" width="300px;"/><br /><sub><b>Wave on Apple Watch</b></sub></a>    
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloParticle"><img src="documents/iOS.particle.gif" width="300px;"/><br /><sub><b>Particle on iPhone</b></sub></a>
  </tr>
</table>

## 📞Community Channel
Thanks for the help from the community, you all make GUI-lite better! And welcome to any new friend to join us.
- [**@Twitter**](https://twitter.com/idea4good)
- QQ group code:
<table>
  <tr>
    <td align="center"><a href="https://jq.qq.com/?_wv=1027&k=O714kulz"><img src="documents/qq.group-5.png" width="240px;"/><br /><sub><b>開發群🔑：616696441</b></sub></a>
    <td align="center"><img src="documents/VXGZH.jpg" width="240px;"/><br /><sub><b>公众号🔑：阿布编程</b></sub>
  </tr>
</table>
