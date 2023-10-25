#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 320

#define TRUE 1
#define FALSE 0

char display[64][32]= {"1", "1", "1"};

unsigned short int quit = FALSE;

int main(){

	SDL_Window *window = NULL;

	SDL_Surface *screenSurface = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("Erro: %s\n", SDL_GetError());
	}else{
		window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
		if(window == NULL)
			printf("Erro na janela: %s\n", SDL_GetError());
		else{
			screenSurface = SDL_GetWindowSurface(window);	
			SDL_FillRect(screenSurface, NULL, 0xfffffff);
			SDL_UpdateWindowSurface(window);

			SDL_Event event;

			while(quit == FALSE){

				while(SDL_PollEvent(&event)){

					if(event.type == SDL_QUIT)
						quit = TRUE;
					if(event.key.keysym.sym == SDLK_ESCAPE)
						quit = TRUE;
					

				}

			}
			
			
			/*
			printf("test\n");
			SDL_Rect rectang;
			rectang.w = 10;
			rectang.h = 10;
			for(int j = 0; j < 32; ++j){
				for(int i = 0; i < 64; ++i){
					rectang.x = i * 10;
					rectang.y = j * 10;	
					printf("i = %d, j = %d\n", i, j);
					SDL_FillRect(screenSurface,&rectang, 0x00000000);
					SDL_Delay(10);
					SDL_UpdateWindowSurface(window);
					//printf("i = %d, j = %d\n", i, j);
				}
			}	
			SDL_UpdateWindowSurface(window);
			SDL_Delay(2000);*/
				
		}
	}
	
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
