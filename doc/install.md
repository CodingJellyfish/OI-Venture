# Build Instructions

This project contains as less dependencies as possible. So building it will be simple.

## Linux

You need to install g++, cmake and git at first. Then run these commands:

```bash
git clone https://github.com/CodingJellyfish/OI-Venture.git
cd OI-Venture
mkdir build
cd build
cmake ..
make
```

After that there'll be an executable named OI-Venture in the build directory.

## Windows

You need to install mingw-w64 and cmake and add to system path at first. Then run these commands:

```bash
git clone https://github.com/CodingJellyfish/OI-Venture.git
cd OI-Venture
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```