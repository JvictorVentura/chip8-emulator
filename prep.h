#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#define MAX_WIDTH 64
#define MAX_HEIGHT 32

#define ON 1
#define OFF 0

#define TRUE 1
#define FALSE 0

typedef unsigned short int byte2;
typedef unsigned char byte;
 
byte  V[16];
byte2 I;
byte delay_timer;
byte sound_timer;
unsigned short int PC;
unsigned short int stack[16];
byte stack_pointer;
byte Key = 0;
byte2 opcode = 0;
 
  byte ram[4096]= {
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
 
byte display[MAX_WIDTH][MAX_HEIGHT];

byte Check_least_significant_bit( byte var ){
	if( (var & 0x1) == 0x1)
		return 1;
	else
		return 0;
}

byte Check_most_significant_bit( byte var ){
	if( (var & 0x80) == 0x80)
		return 1;
	else
		return 0;
}

//void draw_line( byte coordinate_X, byte coordinate_Y, byte2 adress){}

byte Get_Key(){
	byte quit = FALSE;
	SDL_Event even;

	while( quit == FALSE){
		while(SDL_PollEvent(&even)){
			if(even.type == SDL_QUIT)
				quit = TRUE;
			
			switch(even.key.keysym.sym){
				case SDLK_1:
					return 0x1;
					break;
				case SDLK_2:
					return 0x2;
					break;
				case SDLK_3:
					return 0x3;
					break;
				case SDLK_4:
					return 0xC;
					break;
				case SDLK_q:
					return 0x4;
					break;
				case SDLK_w:
					return 0x5;
					break;
				case SDLK_e:
					return 0x6;
					break;
				case SDLK_r:
					return 0xD;
					break;
				case SDLK_a:
					return 0x7;
					break;
				case SDLK_s:
					return 0x8;
					break;
				case SDLK_d:
					return 0x9;
					break;
				case SDLK_f:
					return 0xE;
					break;	
				case SDLK_z:
					return 0xA;
					break;
				case SDLK_x:
					return 0x0;
					break;
				case SDLK_c:
					return 0xB;
					break;
				case SDLK_v:
					return 0xF;
					break;

			}
			
		

		}
	}
	
	return 0x10;
}

byte2 Get_Adress( byte2 opcode ){
	return opcode & 0x0FFF;
}

byte Get_Byte( byte2 opcode ){
	return opcode & 0x00FF;
}

byte Get_RegisterX( byte2 opcode ){
	return (opcode & 0x0F00) >> 8;
}

byte Get_RegisterY( byte2 opcode ){
	return (opcode & 0x00F0) >> 4;
}

byte Get_Last_Bits( byte2 opcode){
	return opcode & 0x000F;
}

void update_Key_Pressed(){
	SDL_Event event;

		while(SDL_PollEvent(&event)){
			if(event.key.state == SDL_RELEASED)
				Key = 0x0;	
			else{
				switch(event.key.keysym.sym){
					case SDLK_1:
						Key =  0x1;
					break;
					case SDLK_2:
						Key = 0x2;
					break;
					case SDLK_3:
						Key = 0x3;
					break;
					case SDLK_4:
						Key = 0xC;
					break;
					case SDLK_q:
						Key = 0x4;
					break;
					case SDLK_w:
						Key = 0x5;
					break;
					case SDLK_e:
						Key = 0x6;
					break;
					case SDLK_r:
						Key = 0xD;
					break;
					case SDLK_a:
						Key = 0x7;
					break;
					case SDLK_s:
						Key = 0x8;
					break;
					case SDLK_d:
						Key = 0x9;
					break;
					case SDLK_f:
						Key = 0xE;
					break;	
					case SDLK_z:
						Key = 0xA;
					break;
					case SDLK_x:
						Key = 0x0;
					break;
					case SDLK_c:
						Key = 0xB;
					break;
					case SDLK_v:
						Key = 0xF;
					break;
					case SDLK_ESCAPE:
						Key = 0x10;					
					break;
				}
			}

	}

}
