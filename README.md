# Welcome to GuiLite
<p align="center">
    <img src="documents/logo.png" alt="Logo"/>
</p>
<p align="center">
  <img src="https://img.shields.io/badge/build-passing-brightgreen.svg">
  <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS%20%7C%20iOS%20%7C%20Android%20%7C%20MCU-brightgreen.svg">
  <img src="https://img.shields.io/badge/architecture-amd64%20%7C%20arm%20%7C%20arm64-blue.svg">
  <img src="https://img.shields.io/badge/license-Apache%202-blue.svg">
</p>
<p align="center">The smallest header-only GUI library (5 KLOC) for all platforms.</p>
<p align="center">
    <img src="documents/WhyGuiLite.png" alt="Why GuiLite"/>
</p>

- [中文](README_zh.md)

## Lightweight
- ✂️Small: Just under 5,000 lines of C++ and header-only: GuiLite.h
- ⚡Fast: Render a GUI within one invocation, independent of any OS or 3rd party library
- 💉Embeddable: Runs inside Qt/MFC/Winform/Cocoa/Web - Keep legacy Qt/MFC code reusable
- ⚙️️Hardware Minimum Requirements:

    | Processor | Disk/ROM space | Memory |
    | --- | --- | --- |
    | 24 MHZ | 29 KB | 9 KB |

## Cross platform
- Supported OSes: iOS/macOS/WathOS, Android, Linux, Windows, RTOS... or **MCU without OS**
- Supported languages: C/C++, Swift, Java, Javascript, C#, Golang...
- Supported 3rd party libraries: Qt, MFC, Winforms, CoCoa...

## Useful features
- 🔣Multi-language, supports UTF-8
- 🔨[Toolkit](https://github.com/idea4good/GuiLiteToolkit) for building font/image resources
- 📐[Layout GUI WYSIWYG](https://github.com/idea4good/GuiLitePreviewer)
- ☁️Cloud + IoT Solution
- 📊Code Telemetry and Analysis in real time
- 📦Supports 3D and [Web](https://github.com/idea4good/GuiLiteWeb)
- 🐋Run in docker with a single command: `sudo docker run -it --privileged -v /dev:/dev-share idea4good/gui-lite:latest bash /run.sh`

## Easy to learn and support
Even a C beginner could master GuiLite quickly. The source code only uses basic C++ features (class, virtual function). We chose C++ as it could make the code size significantly smaller and easier to read.
- 📚Documentation
    - [How to use GuiLite?](documents/HowToUse.md)
    - [Design specification](documents/HowToWork.md)
    - [How to Layout widgets?](documents/HowLayoutWork.md)
    - [How to build unicode font/bitmap resource?](https://github.com/idea4good/GuiLiteToolkit)
    - [How to switch theme?](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/UIcode/source/resource/resource.cpp)
    - [How to dispatch messages?](documents/HowMessageWork.md)
    - [UML chart of GuiLite core](documents/UML.md)
- 📈Learning steps
    1. Build GuiLite library
    2. Build/Debug HelloXXX demos
    3. Read/Modify `HelloXXX/UIcode/UIcode.cpp` code
    4. Read/Modify `widgets` code
    5. Read/Modify `core` code
    6. Build your GUI framework
- 📞[Reach out us](#community-channel) if you have any questions you are welcomed to our developer family.
- 🀄️[Mirror repository in China](https://gitee.com/idea4good/GuiLite)

## Demo wall
Click the image you like, and run it on your hardware
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
  <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloTimer"><img src="documents/HelloTimer.gif" width="200px;"/><br /><sub><b>Timer</b></sub></a>
  <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloMolecule"><img src="documents/HelloMolecule.gif" width="200px;"/><br /><sub><b>Molecule move</b></sub></a>
  <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloPendulum"><img src="documents/HelloPendulum.gif" width="200px;"/><br /><sub><b>Pendulum effect</b></sub></a>
  <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloSlide"><img src="documents/HelloSlide.gif" width="200px;"/><br /><sub><b>Swipe view</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/Hello3D"><img src="documents/iWatch.hello3D.gif" width="200px;"/><br /><sub><b>3D on Apple Watch</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloWave"><img src="documents/iWatch.helloWave.gif" width="200px;"/><br /><sub><b>Wave on Apple Watch</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloParticle"><img src="documents/iOS.particle.gif" width="200px;"/><br /><sub><b>Particle on iPhone</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/iOS.gif" width="200px;"/><br /><sub><b>HostMonitor on iPhone</b></sub></a>
  </tr>
</table>

<table>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLitePreviewer"><img src="documents/Previewer.gif" width="600px;"/><br /><sub><b>Layout GUI</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloGuiLite"><img src="documents/HelloGuiLite.gif" width="600px;"/><br /><sub><b>Run Windows on Linux</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloFont"><img src="documents/HelloFont.gif" width="600px;"/><br /><sub><b>Multi-language</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/BuildInfo.png" width="600px;"/><br /><sub><b>Code Telemetry and Analysis in real time</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloScroll"><img src="documents/HelloScroll.gif" width="600px;"/><br /><sub><b>Scroll widget</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloTransparent"><img src="documents/HelloTransparent.gif" width="600px;"/><br /><sub><b>Transparent dialog</b></sub></a>
  </tr>
</table>

<table>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloNets"><img src="documents/HelloNets-pc.gif" width="300px;"/><br /><sub><b>3D Nets on Windows/Linux</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HelloWidgets"><img src="documents/HelloWidgets.gif" width="300px;"/><br /><sub><b>How to use widgets</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/unicode.jpg" width="300px;"/><br /><sub><b>Multi-language</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteWeb"><img src="documents/GuiLite3D.gif" width="300px;"/><br /><sub><b>3D on Web</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteWeb"><img src="documents/GuiLiteCube.gif" width="300px;"/><br /><sub><b>3D on Web</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteWeb"><img src="documents/GuiLiteGraphic.gif" width="300px;"/><br /><sub><b>Tutorial on Web</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/HostMonitor.gif" width="300px;"/><br /><sub><b>HostMonitor on Windows</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/data_on_cloud.png" width="300px;"/><br /><sub><b>HostMonitor on IoT-Cloud</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/Android.gif" width="300px;"/><br /><sub><b>HostMonitor on Android</b></sub></a>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/WinMR.gif" width="300px;"/><br /><sub><b>HostMonitor on Windows Mixed Reality</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/Mac.gif" width="300px;"/><br /><sub><b>HostMonitor on Mac</b></sub></a>
    <td align="center"><a href="https://github.com/idea4good/GuiLiteSamples/tree/master/HostMonitor"><img src="documents/Linux.gif" width="300px;"/><br /><sub><b>HostMonitor on Linux</b></sub></a>
  </tr>
</table>

Most of the demos have about 100 lines of UI code, more details [here.](https://github.com/idea4good/GuiLiteSamples)

## 📞Community Channel
Thanks for the help from the community, you all make GuiLite better! And welcome to any new friend to join us.
- [**@Twitter**](https://twitter.com/idea4good)
- QQ group code:
<table>
  <tr>
    <td align="center"><a href="https://jq.qq.com/?_wv=1027&k=qfvGGAa7"><img src="documents/qq.group-1.jpg" width="240px;"/><br /><sub><b>大群：527251257</b></sub></a>
    <td align="center"><a href="https://jq.qq.com/?_wv=1027&k=yKnUJj1B"><img src="documents/qq.group-2.jpg" width="240px;"/><br /><sub><b>中群：841031761</b></sub></a>
    <td align="center"><a href="https://jq.qq.com/?_wv=1027&k=5hc3qwj"><img src="documents/qq.group-3.jpg" width="240px;"/><br /><sub><b>小群：717471210</b></sub></a>
  </tr>
</table>
