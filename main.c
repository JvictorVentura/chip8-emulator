#include <stdio.h>
#include "opcodes.h"

void fetch_opcode(){
	opcode = (ram[PC] << 8) | ( ram[PC + 1]);
	PC += 2;
}

void print_instruction(){

	switch(opcode & 0xF000){
		case 0x0000:
				switch(opcode & 0x0FFF){
					case 0x00E0:
								printf("CLEAR_DISPLAY();");
					break;
					case 0x00EE:
								printf("RETURN_SUBROUTINE();");
					break;
				}	
		break;
		
		case 0x1000:
				printf("JMP(address);");
		break;

		case 0x2000:
				printf("CALL_SUBROUTINE(address);");
		break;
		
		case 0x3000:
				printf("IF_BYTE(X, byte);");
		break;
		
		case 0x4000:
				printf("IF_NOT_BYTE(X, byte);");
		break;

		case 0x5000:
				printf("IF_VAR(X, Y);");
		break;
		
		case 0x9000:
				printf("IF_NOT_VAR(X, Y);");
		break;

		case 0x6000:
				printf("LOAD_BYTE(X, byte);");
		break;

		case 0x7000:
				printf("ADD(X, byte);");
		break;

		case 0x8000:
				switch(opcode & 0x000F){
						case 0x0000:
								printf("LOAD_VAR(X, Y);");	
						break;	
						
						case 0x0001:
								printf("OR(X, Y);");
						break;	
						
						case 0x0002:
								printf("AND(X, Y);");
						break;
						
						case 0x0003:
								printf("XOR(X, Y);");
						break;
						
						case 0x0004:
								printf("ADD_CARRY(X, Y);");
						break;
	
						case 0x0005:
								printf("SUB_Y_FROM_X(X, Y);");
						break;
							
						case 0x0007:
								printf("SUB_X_FROM_Y(X, Y);");
						break;

						case 0x0006:
								printf("SHIFT_RIGHT(X);");
						break;

						case 0x000E:
								printf("SHIFT_LEFT(X);");
						break;
						
				}		
		
		break;

		case 0xA000:
				printf("LOAD_ADDRESS(address);");	
		break;
		
		case 0xB000:
				printf("JUMP2(address);");
		break;

		case 0xC000:
				printf("RAND( X, byte);");
		break;

		case 0xD000:
				printf("DRAW(X, Y, lastBits, I);");
		break;

		case 0xE000:
				switch(opcode & 0x00FF){
	
					case 0x009E:
							printf("IS_KEY_PRESSED(X);");	
					break;

					case 0x00A1:
							printf("IS_KEY_NOT_PRESSED(X);");
					break;

				}
		break;
		case 0xF000:
				switch(opcode & 0x00FF){

					case 0x0007:
							printf("GET_DELAY(X);");
					break;
	
					case 0x000A:
							printf("WAIT_FOR_KEY(X);");	
					break;

					case 0x0018:
							printf("SET_SOUND_TIMER(X);");
					break;

					case 0x001E:
							printf("ADD_TO_ADDRESS(X);");
					break;

					case 0x0029:
							printf("SET_ADDRESS_SPRITE(X);");
					break;

					case 0x0033:
							printf("BINARY_CODED_DECIMAL(X);");
					break;

					case 0x0055:
							printf("DUMP_REGISTERS(X);");
					break;

					case 0x0065:
							printf("LOAD_REGISTERS(X);");
					break;

					case 0x0015:
							printf("SET_DELAY(X);");
					break;

				}
			break;	
		
		default:
			printf("opcode nao reconhecido\n");
	}

		printf("\n");

}

void decode_and_execute_opcode(){
	uint8_t X = Get_RegisterX(opcode);
	uint8_t Y = Get_RegisterY(opcode);
	uint8_t byte = Get_Byte(opcode);
	uint16_t address = Get_Address(opcode);	
	uint8_t lastBits = Get_Last_Bits(opcode); 
	
//	print_instruction();
	
	switch(opcode & 0xF000){
		case 0x0000:
				switch(opcode & 0x0FFF){
					case 0x00E0:
								CLEAR_DISPLAY();
					break;
					case 0x00EE:
								RETURN_SUBROUTINE();
					break;
				}	
		break;
		
		case 0x1000:
				JMP(address);
		break;

		case 0x2000:
				CALL_SUBROUTINE(address);
		break;
		
		case 0x3000:
				IF_BYTE(X, byte);
		break;
		
		case 0x4000:
				IF_NOT_BYTE(X, byte);
		break;

		case 0x5000:
				IF_VAR(X, Y);
		break;
		
		case 0x9000:
				IF_NOT_VAR(X, Y);
		break;

		case 0x6000:
				LOAD_BYTE(X, byte);
		break;

		case 0x7000:
				ADD(X, byte);
		break;

		case 0x8000:
				switch(opcode & 0x000F){
						case 0x0000:
								LOAD_VAR(X, Y);	
						break;	
						
						case 0x0001:
								OR(X, Y);
						break;	
						
						case 0x0002:
								AND(X, Y);
						break;
						
						case 0x0003:
								XOR(X, Y);
						break;
						
						case 0x0004:
								ADD_CARRY(X, Y);
						break;
	
						case 0x0005:
								SUB_Y_FROM_X(X, Y);
						break;
							
						case 0x0007:
								SUB_X_FROM_Y(X, Y);
						break;

						case 0x0006:
								SHIFT_RIGHT(X);
						break;

						case 0x000E:
								SHIFT_LEFT(X);
						break;
						
				}		
		
		break;

		case 0xA000:
				LOAD_ADDRESS(address);	
		break;
		
		case 0xB000:
				JUMP2(address);
		break;

		case 0xC000:
				RAND( X, byte);
		break;

		case 0xD000:
				DRAW(X, Y, lastBits, I);
		break;

		case 0xE000:
				switch(opcode & 0x00FF){
	
					case 0x009E:
							IS_KEY_PRESSED(X);	
					break;

					case 0x00A1:
							IS_KEY_NOT_PRESSED(X);
					break;

				}
		break;
		case 0xF000:
				switch(opcode & 0x00FF){

					case 0x0007:
							GET_DELAY(X);
					break;
	
					case 0x000A:
							WAIT_FOR_KEY(X);	
					break;

					case 0x0018:
							SET_SOUND_TIMER(X);
					break;

					case 0x001E:
							ADD_TO_ADDRESS(X);
					break;

					case 0x0029:
							SET_ADDRESS_SPRITE(X);
					break;

					case 0x0033:
							BINARY_CODED_DECIMAL(X);
					break;

					case 0x0055:
							DUMP_REGISTERS(X);
					break;

					case 0x0065:
							LOAD_REGISTERS(X);
					break;

					case 0x0015:
							SET_DELAY(X);
					break;

				}
			break;	
		
		default:
			printf("opcode nao reconhecido\n");
	}
	
}

void draw_screen( SDL_Surface *screenSurface ){
		SDL_Rect pixel;
		pixel.w = 10;
		pixel.h = 10;

		for( uint8_t y = 0; y < MAX_HEIGHT; ++y ){
			for( uint8_t x = 0; x < MAX_WIDTH; ++x){
				pixel.x = x * 10;
				pixel.y = y * 10;
				if( display[x][y] == ON){
					SDL_FillRect( screenSurface, &pixel, 0xFFFFFF);
				}else
					SDL_FillRect( screenSurface, &pixel, 0x0);
			}		
		}	
}

void load_rom( FILE *arq ){
		char c = 'a';
		int i = 0x200;

		fseek(arq, 0, SEEK_END);
		unsigned short int size = ftell(arq);
		fseek(arq, 0, SEEK_SET);

		do{
				c = fgetc(arq);

				ram[i] = c;
				++i;
				--size;

		}while(size > 0);
}

void clean_surface(SDL_Surface *screenSurface){
	SDL_Rect clean;
	clean.w = 640;
	clean.h = 320;
	
	clean.x = 0;
	clean.y = 0;
	SDL_FillRect( screenSurface, &clean, 0x0);	
}


int main(int argc, char **argv){
	
	srand(time(0));

	FILE *arq = NULL;

	if(argc == 2){
		arq = fopen(argv[1], "r");
	}
	
	
	if(arq == NULL){
		printf("Erro ao abrir\n");
		return 1;
	}else
		load_rom( arq );
	PC = 0x200;

	fclose(arq);



	const short int frequency = 500;

	SDL_Window *window = NULL;

	SDL_Surface *screenSurface = NULL;

	if(SDL_Init(SDL_INIT_VIDEO ) < 0){
			printf("Erro: %s\n", SDL_GetError());			
	}else{
			window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MAX_WIDTH * 10, MAX_HEIGHT * 10, SDL_WINDOW_SHOWN);
			if(window == NULL){
				printf("Erro na janela: %s\n", SDL_GetError());
			}else{

				screenSurface = SDL_GetWindowSurface(window);

				CLEAR_DISPLAY();
			 	uint8_t quit = FALSE;
				uint8_t cycle = 0;
				uint8_t timer_cycle = 0;

				while(quit == FALSE){

					update_Key_Pressed(&quit);
					fetch_opcode();
					decode_and_execute_opcode();
					if(update_screen == 1){

						draw_screen( screenSurface );
						SDL_UpdateWindowSurface( window );

						update_screen = 0;
					}
						
					

					if(Key == 0x10)
						quit = TRUE;
				
					if( PC >= 4096)
						quit = TRUE;

					SDL_Delay(1000/frequency);

					if(cycle >= 100){
						cycle = 0;
						timer_cycle = 0;	
					}else{
						++cycle;
					}
					
					if(timer_cycle < 60){
						if(delay_timer > 0) --delay_timer;
						if(sound_timer > 0) --sound_timer;	
						++timer_cycle;
					}
				}
			}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();	

	return 0;
}


