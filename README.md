# Beginnings of a simple text editor

Not sure what this project will be yet fully, but it is supposed be a simple GUI text editor for simple tasks. Intention is to replace gedit with some addditional juice like fuzzy file open and grepping, also simpler UI with no clutter. Not intended to replace vscode as that is already quite good honestly. Just if I would want some quick editing without code completion and didn't want to use vim, this would be the goto editor for that. Compared to vscode advantages should be fast bootup, snappy to use etc, while keeping it simple. Should be very user friendly as well, same as vscode.


## Cross compiling for Windows from Ubuntu Linux

Install `x86_64-w64-mingw32` cross compile toolchain. Make sure you use `sudo update-alternatives --config x86_64-w64-mingw32-g++` and set the default mingw32 g++ compiler option to posix, or whatever would be similar on your system. Use `cmake/mingw-w64-x86_64.cmake` cmake toolchain file when compiling. Also you have to manually add `-static-libgcc` and `-static-libstdc++` to the cmake compile flags (so add to `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`). Once the compile is done you have to manually copy all .dll files from the resulting build folder (e.g. SDL2.dll) to the source root and copy `/usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll` to the source root. You can test the result with wine64. To also check what .dll files the .exe is depending on you can use `x86_64-w64-mingw32-objdump -p *.exe | grep dll`.
