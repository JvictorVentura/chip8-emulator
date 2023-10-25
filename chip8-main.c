#include <stdio.h>
#include "opcodes.h"

void fetch_opcode(){
	opcode = (ram[PC] << 8) | ( ram[PC + 1]);
	PC += 2;
}

void decode_and_execute_opcode(){
	byte X = Get_RegisterX( opcode );
	byte Y = Get_RegisterY( opcode );
	byte __byte = Get_Byte( opcode );
	byte2 adress = Get_Adress( opcode );	
	byte lastBits = Get_Last_Bits( opcode ); 
	
	switch( opcode & 0xF000 ){
		case 0x0000:
				switch( opcode & 0x0FFF ){
					case 0x00E0:
								CLEAR_DISPLAY();
					break;
					case 0x00EE:
								RETURN_SUBROUTINE();
					break;
				}	
		break;
		
		case 0x1000:
				JMP( adress );
		break;

		case 0x2000:
				CALL_SUBROUTINE( adress );
		break;
		
		case 0x3000:
				IF_BYTE( X, __byte );
		break;
		
		case 0x4000:
				IF_NOT_BYTE( X, __byte );
		break;

		case 0x5000:
				IF_VAR( X, Y );
		break;
		
		case 0x9000:
				IF_NOT_VAR( X, Y );
		break;

		case 0x6000:
				LOAD_BYTE( X, __byte );
		break;

		case 0x7000:
				ADD( X, __byte );
		break;

		case 0x8000:
				switch( opcode & 0x000F ){
						case 0x0000:
								LOAD_VAR( X, Y );	
						break;	
						
						case 0x0001:
								OR( X, Y );
						break;	
						
						case 0x0002:
								AND( X, Y );
						break;
						
						case 0x0003:
								XOR( X, Y );
						break;
						
						case 0x0004:
								ADD_CARRY( X, Y );
						break;
	
						case 0x0005:
								SUB_Y_FROM_X( X, Y );
						break;
							
						case 0x0007:
								SUB_X_FROM_Y( X, Y );
						break;

						case 0x0006:
								SHIFT_RIGHT( X );
						break;

						case 0x000E:
								SHIFT_LEFT( X );
						break;
						
				}		
		
		break;

		case 0xA000:
				LOAD_ADRESS( adress );	
		break;
		
		case 0xB000:
				JUMP2( adress );
		break;

		case 0xC000:
				RAND( X, __byte);
				printf("numero aleatorio = %d\n", V[X]);
				printf("byte = %02x\n\n", __byte);
		break;

		case 0xD000:
				DRAW( X, Y, lastBits, I);
		break;

		case 0xE000:
				switch( opcode & 0x00FF){
	
					case 0x009E:
							IS_KEY_PRESSED( X );	
					break;

					case 0x00A1:
							IS_KEY_NOT_PRESSED( X );
					break;

				}

		case 0xF000:
				switch( opcode & 0x00FF){

					case 0x0007:
							GET_DELAY( X );
					break;
	
					case 0x000A:
							WAIT_FOR_KEY( X );	
					break;

					case 0x0018:
							SET_SOUND_TIMER( X );
					break;

					case 0x001E:
							ADD_TO_ADRESS( X );
					break;

					case 0x0029:
							SET_ADRESS_SPRITE( X );
					break;

					case 0x0033:
							BINARY_CODED_DECIMAL( X );
					break;

					case 0x0055:
							DUMP_REGISTERS( X );
					break;

					case 0x0065:
							LOAD_REGISTERS( X );
					break;

				}
			break;	

		break;

		default:
			printf("opcode nao reconhecido\n");
	}

}

void draw_screen( SDL_Surface *screenSurface ){
		SDL_Rect pixel;
		pixel.w = 10;
		pixel.h = 10;

		for( byte i = 0; i < HEIGHT; ++i ){
			for( byte j = 0; j < WIDTH; ++j){
				pixel.x = j * 10;
				pixel.y = i * 10;
				if( display[i][j] == ON){
					SDL_FillRect( screenSurface, &pixel, 0xFFFFFF);
				}else
					SDL_FillRect( screenSurface, &pixel, 0x0);
			}		
		}	
}

void load_rom( FILE *arq ){
		char c = 'a';
		//char b = 'a';
		int i = 0x200;

		fseek(arq, 0, SEEK_END);
		unsigned short int size = ftell(arq);
		fseek(arq, 0, SEEK_SET);

		do{
				c = fgetc(arq);

				/*if( c == EOF){
					b = c;
					if ( (c = fgetc(arq)) != EOF){
						ram[i] = b;
						++i;
						ram[i] = c;	
					}
				}else
					ram[i] = c; 
				*/

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
	}else{
		arq = fopen("./test_opcode.ch8", "r");
	}
	
	
	if(arq == NULL){
		printf("Erro ao abrir\n");
		return 1;
	}else
		load_rom( arq );
	PC = 0x200;

	fclose(arq);

/*	byte v = 0;
	for(int i = 512; i < 4096; ++i){
		++v;
		printf("%02x ", ram[i]);	
		if( v == 16){
			printf("\n");
			v = 0;
		}
	}*/

	SDL_Window *window = NULL;

	SDL_Surface *screenSurface = NULL;

	if(SDL_Init(SDL_INIT_VIDEO < 0)){
			printf("Erro: %s\n", SDL_GetError());			
	}else{
			window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH * 10, HEIGHT * 10, SDL_WINDOW_SHOWN);
			if(window == NULL){
				printf("Erro na janela: %s\n", SDL_GetError());
			}else{

				screenSurface = SDL_GetWindowSurface(window);

				CLEAR_DISPLAY();
				unsigned short int quit = FALSE;

				while(quit == FALSE){

					update_Key_Pressed();
					fetch_opcode();
					decode_and_execute_opcode();
					draw_screen( screenSurface );
					SDL_UpdateWindowSurface( window );	
					

					if(Key == 0x10)
						quit = TRUE;
				
					if( PC >= 4096)
						quit = TRUE;

					SDL_Delay(3);
					
				}
			}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();	

	return 0;
}


