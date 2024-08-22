//Put these DLLs in game .exe folder for release(user) build to work - NOT NEEDED ANYMORE.
These libs are for mingw64(GCC) compiler
Libs can be statically linked now! I guess it only needs "libstdc++.a" and hopefully it won't need anything else later.
"libstdc++.a"     & "libgcc_eh.a"   is taken from D:\Libs\mingw64\lib\gcc\x86_64-w64-mingw32\13.2.0
"libstdc++-6.dll" & "libgcc_s_seh-1.dll" are from D:\Libs\mingw64\bin

I found out how to link statically via gcc command flags, so no .a/.dll files needed anymore.
https://stackoverflow.com/questions/18138635/mingw-exe-requires-a-few-gcc-dlls-regardless-of-the-code

Okay well, Dependency Walker shows some errors so we maybe need to change mingw compiler to msvcrt version. - No, it's fine, I tried other programs, there were the same errors.