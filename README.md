# cpp_textrendering

Basic playground project to test out text rendering with SDL2, and other C++ stuff


## Cross compiling for Windows from Ubuntu Linux

Install `x86_64-w64-mingw32` cross compile toolchain. Make sure you use `sudo update-alternatives --config x86_64-w64-mingw32-g++` and set the default mingw32 g++ compiler option to posix, or whatever would be similar way to do that on your system. Use `cmake/mingw-w64-x86_64.cmake` cmake toolchain file when compiling, I found the easiest is to use cmake-gui for this. Once the compile is done you have to manually copy all .dll files from the resulting build folder (e.g. SDL2.dll) to the source root and copy `/usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll` to the source root (or wherever you are running the program from). You can test the result with wine64. To also check what .dll files the .exe is depending on you can use `x86_64-w64-mingw32-objdump -p *.exe | grep dll`.
