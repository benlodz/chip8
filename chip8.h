#include <string>
#include <SDL2/SDL.h>
#pragma once

class chip8{
private:



  unsigned char V[17]; // Registers
  unsigned short I = 0;
  unsigned short PC = 512;

  unsigned short temp_op;
  unsigned short nnn; // 12 bit value

  unsigned char fst_nib; // first digit in the opcode
  unsigned char end_nib; // last digit in opcode

  unsigned char kk;
  unsigned char x; // 4 bit value
  unsigned char y;

  unsigned short pkop;

  unsigned short stack[16];
  unsigned short sp;

  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Renderer *renderer;
  SDL_Event event;

  int quit = 0;
  bool keyboard[16];

  unsigned char delay_timer;
  unsigned char sound_timer;


  int xcordinate;
  int ycordinate;


  bool *pixel;
  unsigned short drw_addr;



  SDL_Rect *display;

  int convcrt(int x, int y);
  unsigned short tempc;


  unsigned char font[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};



public:
  unsigned char *memory = new unsigned char[4096];
  unsigned short opcode;
  void cycle();
  void load_file(std::string filez);
  void init_graphics();
  void fetch_opcode();
  void execute_opcode();
  void shutdown();
  void draw();
  void run();

  bool drawFlag = false;
};
