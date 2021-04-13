## 使用Gradle编译Guilite ###

* ### Windows平台: ###
    * 兼容VS各大版本，自动选择最新版本。
    * 兼容Cygwin或MinGW，确保gcc在path中。目前不支持Clang。
    * 按`win+R`输入`cmd`打开命令行运行：`g++ -v`，有打印出版本则正常。
    * 编译Guilite：
    ```powershell
    cd $GUILITE\workspace\Gradle\
    gradlew produce
    ```
    * 输出位置：$GRADLE\build\lib\main\release\shared(static)\
    
* ### Linux和Mac平台: ###
    * 将使用系统PATH发现GCC或Clang，请确保已经安装GCC或Clang。
    * 打开终端运行：`g++ -v`，有打印出版本则正常。
    * 编译Guilite：
    ```shell
    cd $GUILITE/workspace/Gradle/
    gradlew produce
    ```
    * 输出位置：$GRADLE/build/lib/main/release/shared(static)/
    
对单片机系统不了解，未支持！