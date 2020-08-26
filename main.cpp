#include <iostream>
#include <fstream>
#include <string>
#include "chip8.h"
#include <SDL2/SDL.h>

int main(int argc, char*argv[]){



  chip8 emu;
  std::string filez = argv[1];
  emu.init_graphics();
  emu.load_file(filez);



  //emu.fetch_opcode();
  //emu.execute_opcode();

  /*

  for (int i = 0; i < 185; i++){
    emu.cycle();
    if (emu.drawFlag == true){
      emu.draw();
    }
  }
  */

  emu.run();

  emu.shutdown();




};
