# Chip8-Emulator


My first crack at an emulator and my first actual big C++ project. In order to compile, assuming you're on a system with make, SDL2, and GCC, all you need to do is run the following:

```
git clone https://github.com/MidnightMagician/Chip8-Emulator.git
cd Chip8-Emulator
make
./chip8 <rom file name>
```
The two roms included work and are part of the public domain. Due to the nature of the Chip 8, some roms will not work because of stretching and will cause the program to crash. I'll have this fixed in the future.
