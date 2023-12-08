#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#define MAX_WIDTH 64
#define MAX_HEIGHT 32
#include <stdint.h>

#define ON 1
#define OFF 0

#define TRUE 1
#define FALSE 0

 
uint8_t  V[16];
uint16_t I;
uint8_t delay_timer;
uint8_t sound_timer;
unsigned short int PC;
unsigned short int stack[16];
uint8_t stack_pointer;
uint8_t Key = 0;
uint16_t opcode = 0;

uint8_t update_screen = 0;
 
  uint8_t ram[4096]= {
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
 
uint8_t display[MAX_WIDTH][MAX_HEIGHT];

uint8_t Check_least_significant_bit( uint8_t var ){
	if( (var & 0x1) == 0x1)
		return 1;
	else
		return 0;
}

uint8_t Check_most_significant_bit( uint8_t var ){
	if( (var & 0x80) == 0x80)
		return 1;
	else
		return 0;
}

//void draw_line( uint8_t coordinate_X, uint8_t coordinate_Y, uint16_t address){}

uint8_t Get_Key(){
	uint8_t quit = FALSE;
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

uint16_t Get_Address( uint16_t opcode ){
	return opcode & 0x0FFF;
}

uint8_t Get_Byte( uint16_t opcode ){
	return opcode & 0x00FF;
}

uint8_t Get_RegisterX( uint16_t opcode ){
	return (opcode & 0x0F00) >> 8;
}

uint8_t Get_RegisterY( uint16_t opcode ){
	return (opcode & 0x00F0) >> 4;
}

uint8_t Get_Last_Bits( uint16_t opcode){
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


void get_sprite(uint8_t sprite[], uint8_t sprite_height, uint16_t address){
	for(short i = 0; i < sprite_height; ++i){
		sprite[i] = ram[address + i];
	}
}

void get_area_of_display(uint8_t X_coordinate,uint8_t Y_coordinate, uint8_t area_of_display[], uint8_t height){
	uint8_t temp_line;
	uint8_t X_offset = 0;
	uint8_t Y_offset = 0;

	uint8_t X_final = 0;
	uint8_t Y_final = 0;

  for(uint8_t line = 0; line < height; ++line){
	  temp_line = 0;
		X_offset = 0;

		Y_final = (Y_coordinate + Y_offset) & 31;	// by ANDing with this number, it ensures it will not suprass this value

    for(uint8_t pixel = 0b10000000; pixel > 0; pixel >>= 1){
			X_final = (X_coordinate + X_offset) & 63; // by ANDing with this number, it ensures it will not suprass this value

      if(display[X_final][Y_final] == ON){
        temp_line += pixel;
      }
			X_offset++;
    }
   area_of_display[line] = temp_line;
	 Y_offset++;
  }

}

void XOR_sprite_area_of_display(uint8_t sprite[], uint8_t area_of_display[], uint8_t result[], uint8_t height){
	for(uint8_t line = 0; line < height; ++line){
		result[line] = sprite[line] ^ area_of_display[line];	
	}
}

uint8_t collision_check(uint8_t BEFORE_area_of_display[], uint8_t AFTER_area_of_display[], uint8_t height){
	for(uint8_t line = 0; line < height; ++line){
		for(uint8_t pixel = 0b10000000; pixel > 0; pixel >>= 1){
			if((BEFORE_area_of_display[line] & pixel) == pixel){
				if((AFTER_area_of_display[line] & pixel) != pixel)
					return TRUE;
			}
		}
	}

	return FALSE;
}

void write_to_display(uint8_t X_coordinate, uint8_t Y_coordinate, uint8_t xor_result[], uint8_t height){
	uint8_t X_offset = 0;
	uint8_t Y_offset = 0;

	uint8_t X_final = 0;
	uint8_t Y_final = 0;

	for(uint8_t line = 0; line < height; ++line){
		X_offset = 0;

		Y_final = (Y_coordinate + Y_offset) & 31;	// by ANDing with this number, it ensures it will not suprass this value

		for(uint8_t pixel = 0b10000000; pixel > 0; pixel >>= 1){
			X_final = (X_coordinate + X_offset) & 63;	// by ANDing with this number, it ensures it will not suprass this value
			
			if( (xor_result[line] & pixel) == pixel)
				display[X_final][Y_final] = ON;
			else
				display[X_final][Y_final] = OFF;

			X_offset++;
		}
		Y_offset++;
	}
}
