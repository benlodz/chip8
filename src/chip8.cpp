#include "chip8.h"
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <bitset>
//#include "chip8graphics.h"
//

#define SCALE 4

void chip8::fetch_opcode(){
  opcode = (memory[PC] << 8) + memory[PC+1];

  pkop = (memory[PC+2] << 8) + memory[PC+1];
  printf("OP:%x|ADDR:%x|PK:%x| " ,opcode,PC,pkop);

  fst_nib = memory[PC] >> 4; // first number in opcode to determine
  //printf("%x",fist_nib);      // what to execute
  //parsing of the opcode here
  nnn = opcode & 0x0FFF;
  x = memory[PC] & 0x0F;
  y = memory[PC+1] >> 4;
  kk = memory[PC+1];
  end_nib = memory[PC+1] & 0x0F;

  PC += 2;
};

void chip8::execute_opcode(){

  switch(fst_nib){

    case 0:

    switch (end_nib){
      case 0:
      printf("00E0 | clears screen\n");
      for (int j = 0; j <= 31; j++){
        for (int i = 0; i <= 63; i++){
          pixel[convcrt(i,j)] = false;
        }
      }
      drawFlag = true;
      break;

      case 0xE:
      printf("00EE| returns from subroutine\n");
      sp--;
      PC = stack[sp];
      break;

    }
    break;

    case 1: // 1nn JP Addr; sets PC to nnn
    printf("1nnn| nnn:%x\n", nnn);
    PC = nnn;
    break;


    case 2: // 2nnn CALL addr; increments stack pointer and sets PC to nnn
    printf("2nnn| subroutine\n");
    stack[sp] = PC;
    sp++;
    PC = nnn;
    break;

    case 3: // SE 3xkk; skips next instruction if Vx == kk
    printf("3xkk| V%x:%x kk:%x\n",x,V[x],kk);
    if (V[x] == kk){
	   PC += 2;
    }
    break;

    case 4: // SNE 4xkk; skips next instruction if Vx != kk
    printf("4xkk\n");
    if (V[x] != kk){
	    PC += 2;
    }
    break;

    case 5: // SE 5xy0; compares register Vx = Vy, skips next instruction
    printf("5xy0\n");
    if (V[x] == V[y]){
	    PC += 2;
  }
    break;

    case 6: // LD 6xkk; puts value kk into register Vx
    V[x] = kk;
    printf("6xkk| V%x ->%x | V%x=%x\n",x,kk,x,V[x]);
    break;

    case 7: // ADD 7xkk; adds kk to register Vx
    printf("7xkk| V%x = Vx+kk| kk:%x Vx:%x | ", x,kk,V[x]);
    V[x] += kk;
    printf("Vx:%x\n",V[x]);
    break;

    case 8:
    switch (end_nib){

	    case 0: // LD 8xy0; stores the value of register Vy into Vx
        printf("8xy0 | V%x:%x V%x:%x |",x,V[x],y,V[y]);
		    V[x] = V[y];
        printf(" -> V%x:%x\n",x,V[x]);
		    break;

	    case 1: // OR 8xy1; performs a bitwise or on  Vx and Vy, then stores the result in Vx
		    V[x] |= V[y];
		    break;

	    case 2: // AND 8xy2; performs a bitwise and on Vx and Vy, then stores the result in Vx
		    V[x] &= V[y];
		    break;

	    case 3: // XOR 8xy3; performs a bitwise exclusive OR on Vx and Vy, then stores the result in Vx
		    V[x] ^= V[y];
		    break;

	    case 4: // ADD 8xy4; adds Vx and Vy and stores it in Vx, if it's greated then 255 then VF is set 1 otherwise 0
        printf("8xy4| V%x:%x V%x:%x",x,V[x],y,V[y]);
        //int sum = V[x] + V[y]
		    if ( (V[x] + V[y]) > 255){
			    V[0xF] = 1;
		    } else{
			    V[0xF] = 0;
		    }
		    V[x] = (V[x] + V[y]) & 0xFF;
		    break;

	    case 5: // SUB 8xy5; if Vx > Vy, VF = 1 otherwise 0, then Vy - Vx is stored in Vx
		    if (V[x] > V[y]){
			    V[0xF] = 1;
		    } else{
			    V[0xF] = 0;
		    }
        V[x] -= V[y];
		    break;

	    case 6: // SHR 8xy6; if the least signficant bit is 1 then VF is set to 1, otherwise 0. Then Vx is divided by 2
		    V[0xF] = V[x] & 0x1;
		    V[x] /= 2;
		    break;

	    case 7: // SUBN 8xy7; if Vy > Vx then VF = 1 otherwise 0, then Vx = Vx - Vy
		    if (V[y] > V[x]){
			    V[0xF] = 1;
		    } else {
			    V[0xF] = 0;
		    }
		    V[x] = (V[y] - V[x]);
		    break;

	    case 0xE: // SHL 8xyE; If most signficant bit is 1 then VF =1, else 0, Vx is multiplied by 2
		    V[0xF] = (V[x] & 0x80) >> 7;
		    V[x] *= 2;
		    break;
    }

    case 9: // SNE 9xy0; skips next instruction if Vx != Vy
    if (V[x] != V[y]){
	    PC += 2;
    }
    break;

    case 0xA: // LD Ann; I = nnn
    printf("Annn | nnn:%x -> I | ",nnn);
    I = nnn;
    printf("I:%x\n",I);
    break;

    case 0xB:
    PC = nnn + V[0];
    break;

    case 0xC: // RND Cxkk; Vx = random byte
    V[x] = kk & ( rand() % 256);
    printf("Cxkk| V%x:%x\n",x,V[x]);
    break;

    case 0xD:

    printf("0xDxyn | x:%u | y:%u | height:%u | I: %x |\n ",V[x],V[y],end_nib,I);

    drawFlag = true;
    V[0xF] = 0;

    xcordinate = V[x];
    ycordinate = V[y];

    printf("%u | %u\n",xcordinate,ycordinate);


    for (int yline = 0; yline < end_nib; yline++){
	    drw_addr = memory[I + yline];
	    for ( int xaxis = 0; xaxis < 8; xaxis++){
        if (( (drw_addr << xaxis) & 0x80) != 0){
          if ( pixel[convcrt(xcordinate+xaxis,ycordinate+yline)] == true){
            V[0xF] = 1;
          }

          printf("%u\n",convcrt(xcordinate+xaxis,ycordinate+yline));
          pixel[convcrt((xcordinate+xaxis),(ycordinate+yline))] = !pixel[convcrt(xcordinate+xaxis,ycordinate+yline)];
        }
        }
      }

    break;

    case 0xE:
    switch (end_nib){
      case 0xE: // Ex9E SKP; skip next instruction if V[x] is pressed
      if (keyboard[V[x]] == true){
        PC += 2;
      }
      break;

      case 0x1: // ExA1 SKP; skip next instruction if V[x] is not pressed
      if (keyboard[V[x]] == false){
        PC += 2;
      }
      break;
    }

    case 0xF:

    switch (end_nib){
      case 7: // LD Fx07; set Vx with delay timer value
      V[x] = delay_timer;
      break;

      case 0xA: // LD fx0A; wait for a key press and stores the value into Vx
      PC -= 2;

      for (int i = 0; i <= 16; i++){

        if (keyboard[i] = true){
          V[x] = keyboard[i];
          PC += 2;
          break;
        } else {
        }

      }

      break;

      case 5: // LD fx15; delay timer = V[x];

      if (y == 5){
        printf("fx55|\n");
        for (int i = 0; i <= x; i++){
          memory[I+i] = V[i];
        }

      } else if (y == 6){
        printf("fx65\n");
        for (int i = 0; i <= x; ++i){
          V[i]= memory[I+i];
        }
      } else{
        printf("fx15|\n");
        delay_timer = V[x];
      }
      break;

      case 8: // LD fx18; sound timer = Vx
      break;

      case 0xE: // Fx1E ADD; value of I and a Vx are added
      printf("Fx1E | I += V%x\n",x);
      I += V[x];
      break;

      case 9: // fx29 LD; value of I is set to the memory address for sprite
      printf("fx29| I = memory address for letter\n");
      I = memory[(V[x]*5)];
      break;

      case 3: // Fx33 BD; stores decimal value of Vx in I,I+1,I+2
      memory[I] = V[x] / 100;
      memory[I+1] = (V[x]/10) % 10;
      memory[I+2] = (V[x]) % 10;
      printf("%u | %u | %u | %u\n" , V[x],memory[I],memory[I+1],memory[I+2]);
      break;



    }
    break;

    default:
    std::cout << "UNKNOWN OPCODE" << std::endl;
    break;

  }



};

void chip8::cycle(){
  fetch_opcode();
  execute_opcode();

  if (delay_timer != 0){
    delay_timer -= 1;
  }

};



void chip8::load_file(std::string filename){
  std::ifstream chip8file (filename,std::ios::binary|std::ios::ate);
  std::streampos size;
  char * buffer;

  if (chip8file.is_open()){ // checks if it's open
    std::clog << "File loaded" << std::endl;
    size = chip8file.tellg(); // returns streampos at end of file
    buffer = new char[size]; // creates buffer size of file
    chip8file.seekg(0,std::ios::beg); // puts the streampos at start
    chip8file.read(buffer,size);
    chip8file.close();

    for (long i = 0; i < size; i++){
      memory[PC+i] = buffer[i];
    }
    delete[] buffer;
  }
}

void chip8::init_graphics(){
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    std::cerr << "SDL failed to start!" << std::endl;
  }

  window = SDL_CreateWindow("SDL",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,256,128,SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);


  display = new SDL_Rect[2048];
  pixel = new bool[2048];

  SDL_SetRenderDrawColor( renderer, 0x00 , 0x00 , 0x00 , 0x00 );
  for (int j = 0; j < 31; j++){
    for (int i = 0; i < 63; i++){
      display[ convcrt(i,j)].w = SCALE;
      display[ convcrt(i,j)].h = SCALE;
      display[ convcrt(i,j)].y = j*SCALE;
      display[ convcrt(i,j)].x = i*SCALE;
      pixel[convcrt(i,j)] = false;
      //std::cout << i*2 << "|" << j*2 << std::endl;
      SDL_RenderFillRect( renderer, &display[convcrt(i,j)]);
    }
  }
  SDL_RenderPresent(renderer);

  // clears registers
  for (int i = 0; i <= 16; i++){
    V[i] = 0;
  }

  // clears memory
  for (int i=0; i < 4096; i++){
    memory[i] = 0;
  }

  // loads font set
  for(int i = 0; i < 80; ++i){
    memory[i] = font[i];
  }

  delay_timer = 0;
  sp = 0;
  srand (time(NULL));



}

int chip8::convcrt(int x,int y){
  return( (x + (y*64) ) );
}

// this updates the screen
void chip8::draw(){

  printf("draw... ");

  for (int j = 0; j <= 31; j++){
    for (int i = 0; i <= 63; i++){
      //std::cout << i << "|" << j << "\n";
      //std::cout << pixel[convcrt(i,j)] << std::endl;

      if (pixel[convcrt(i,j)] == false){
        //printf("off\n");
        SDL_SetRenderDrawColor( renderer, 0x00 , 0x00 , 0x00 , 0x00 );
        SDL_RenderFillRect(renderer,&display[convcrt(i,j)]);
      }
      else {
        //printf("on\n");
        SDL_SetRenderDrawColor( renderer, 0xFF , 0xFF , 0xFF , 0xFF );
        SDL_RenderFillRect(renderer, &display[convcrt(i,j)]);

      }
    }
  }
    SDL_RenderPresent(renderer);
    drawFlag = false;

    printf("draw finish\n");
  }

void chip8::run(){
  while (!quit){

    cycle();

    if (drawFlag == true){
      draw();
      drawFlag = false;
    }

    // resets Keyboard
    for (int i = 0; i <=16; i++){
      keyboard[i] = false;
    }


    while (SDL_PollEvent(&event)){


      switch (event.type){
        case SDL_KEYDOWN:
        switch (event.key.keysym.sym){
          case SDLK_1:
          keyboard[1] = true;
          break;
          case SDLK_2:
          keyboard[2] = true;
          break;
          case SDLK_3:
          keyboard[3] = true;
          break;
          case SDLK_4:
          keyboard[0xC] = true;
          break;
          case SDLK_q:
          keyboard[4] = true;
          break;
          case SDLK_w:
          keyboard[5] = true;
          break;
          case SDLK_e:
          keyboard[6] = true;
          break;
          case SDLK_r:
          keyboard[0xD] = true;
          case SDLK_a:
          keyboard[7] = true;
          case SDLK_s:
          keyboard[8] = true;
          break;
          case SDLK_d:
          keyboard[9] = true;
          break;
          case SDLK_f:
          keyboard[0xE] = true;
          break;
          case SDLK_z:
          keyboard[0xA] = true;
          break;
          case SDLK_x:
          keyboard[0] = true;
          break;
          case SDLK_c:
          keyboard[0xB] = true;
          break;
          case SDLK_v:
          keyboard[0xF] = true;
          break;
          case SDLK_p:
          quit = 1;
          break;
          default:
          printf("Problem with keyboard.......");
          break;


        }
      }

      }

    }
  }
















void chip8::shutdown(){
  SDL_Delay(3000);
  delete[] memory;
  delete[] pixel;
  delete[] display;
  SDL_DestroyWindow(window);
}
