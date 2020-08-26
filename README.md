# Chip8-Emulator


My first crack at an emulator and my first actual big C++ project. In order to compile, assuming you're on a system with make, SDL2, and GCC, all you need to do is run the following:

```

git clone https://github.com/MidnightMagician/Chip8-Emulator.git
cd Chip8-Emulator
make
./chip8 <rom file name>
 ```
The two roms included work and were found on some other repo's. Some roms don't work because of stretching and WILL cause a segfault. In the future, I'll probably fix that in the future and get the sound working in addition to that. 
