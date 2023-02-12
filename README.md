# NN-Flappy-Birds
Microsoft Visual Studio 2022 © C++ &amp; SDL2 - Flappy Birds with NN and genetic selection

This is an implementation of a challenge from Mr Daniel Shiffman : https://www.youtube.com/watch?v=c6y21FkaUqw

SDL2 must have been downloaded, Visual Studio © project and Windows © need some configurations :

 - Version of SDL2 used : 2.26.2 (https://github.com/libsdl-org/SDL/releases/download/release-2.26.2/SDL2-devel-2.26.2-VC.zip)
I just decompressed it under C:\Users\pascal\Documents\Visual Studio 2022\libraries

- According to your needs, modify Test SDL2/Test SDL2.vcxproj content to the correct path when you find C:\Users\pascal\Documents\Visual Studio 2022\libraries in it.

 - In Windows environment I added C:\Users\pascal\Documents\Visual Studio 2022\libraries\SDL2-2.26.2\lib\x64 to the PATH environment variable for the programm to reach SDL2.dll (Tune it according with the directory where you decompressed the library).

In Visual Studio ©, just compile and run the program to see a very simple graphic representation of Flappy Birds evolving over generations.
