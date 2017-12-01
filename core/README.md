## How to build core libary for Windows?
depdency: Windows 10, visul stdio 2015/2017
- Open "core.sln" by Visual studio 2017
- Click `build` 

## How to build core libary for Linux（x64 & arm）?
- `cmake .`
- `make`

## How to build core libary for Android?
- `sudo apt-get install gcc-arm-linux-androideabi`
- `cmake cmake -D CMAKE_C_COMPILER="/usr/bin/arm-linux-androideabi-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/arm-linux-androideabi-g++" .`
- `make`

## How to build project using xmake?

Please download and install xmake to build project, you can see [Installation Manual](http://xmake.io/#/home?id=installation)。

#### linux/macosx/windows

```console
$ xmake
```

#### iPhoneos

```console
$ xmake f -p iphoneos
$ xmake
```

#### Android

```console
$ xmake f -p android --ndk=/home/xxx/android-ndk-r10e
$ xmake
```

#### Mingw

```console
$ xmake f -p mingw
$ xmake
```

#### Cross compilation

```console
$ xmake f -p cross --sdk=/usr/local/arm-xxx-gcc/ [--toolchains=/sdk/bin] [--cross=arm-linux-]
$ xmake
```

#### Generate IDE Project Files

```console
$ xmake project -k vs2008
$ xmake project -k vs2017 -m "debug,release"
$ xmake project -k makefile
```

If you want to known more info about xmake, please see [xmake documents](http://xmake.io/#/home).

