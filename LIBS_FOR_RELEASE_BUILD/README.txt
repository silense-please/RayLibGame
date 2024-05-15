Put these DLLs in game .exe folder for release(user) build to work
These libs are for mingw32(GCC) compiler
Libs can be statically linked now! I guess it only needs "libstdc++.a" and hopefully it won't need anything else later.
NOTE - libstdc++.a is a file specifically for release build, for debug lib search in respective mingw debug folder.