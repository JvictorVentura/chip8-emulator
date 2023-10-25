#include "prep.h"

void JMP( byte2 adress ){
	PC = adress;
}
 
void CLEAR_DISPLAY(){
	for(byte i = 0; i < HEIGHT; ++i){
		for(byte j = 0; j < WIDTH; ++j)
			display[i][j] = OFF;
	}
}

void CALL_SUBROUTINE(byte2 adress){
	++stack_pointer;
	stack[stack_pointer] = PC;
	PC = adress;
	
}

void RETURN_SUBROUTINE(){
	PC = stack[stack_pointer];
	--stack_pointer;
}

void IF_BYTE( byte2 X, byte __byte){
	if(V[X] == __byte)
		PC += 2; //talvez mudar dps	
}

void IF_NOT_BYTE( byte2 X, byte __byte){
	if(V[X] != __byte)
		PC += 2; //talvez mudar dps
}

void IF_VAR( byte2 X, byte2 Y){
	if(V[X] == V[Y])
		PC += 2; //talvez mudar dps
}

void IF_NOT_VAR( byte2 X, byte2 Y){
	if(V[X] != V[Y])
		PC += 2; //talvez mudar dps
}

void LOAD_BYTE( byte2 X, byte __byte){
	V[X] = __byte;
}

void LOAD_VAR( byte2 X, byte2 Y){
	V[X] = V[Y];
}

void ADD( byte2 X, byte __byte){
	V[X] += __byte;
}

void ADD_CARRY( byte2 X, byte2 Y){
	if( (V[X] + V[Y]) > 255)
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[X] += V[Y];
}

void OR( byte2 X, byte2 Y){
	V[X] |= V[Y];
}

void AND( byte2 X, byte2 Y){
	V[X] &= V[Y];
}

void XOR( byte2 X, byte2 Y){
	V[X] ^= V[Y];
}

void SUB_Y_FROM_X( byte2 X, byte2 Y){
	if(V[X] > V[Y])
		V[0xF] = 1;
	else
		V[0xF] = 0;	

	V[X] -= V[Y];
}

void SUB_X_FROM_Y( byte2 X, byte2 Y){
	if(V[Y] > V[X])
		V[0xF] = 1;
	else
		V[0xF] = 0;	

	V[X] = V[Y] - V[X];
}

void SHIFT_RIGHT( byte2 X ){
	V[0xF] = Check_least_significant_bit( V[X] );
	V[X] >>= 1;	
}

void SHIFT_LEFT( byte2 X ){
	V[0xF] = Check_most_significant_bit( V[X] );
	V[X] <<= 1;
}

void LOAD_ADRESS( byte2 adress ){
	I = adress;	
}

void JUMP2( byte2 adress){
	PC = V[0] + adress;
}

void RAND( byte2 X, byte __byte ){
	//srand(time(0));
	byte random = rand();
	//V[X] = (rand() % 256) & __byte;
	V[X] = random & __byte;
}

void DRAW( byte X, byte Y, byte2 height, byte2 adress ){

	byte coordinate_X = V[X];
	byte coordinate_Y = V[Y];
	
	for(byte i = 0; i < height; ++i){
		if(coordinate_Y >= HEIGHT)
			coordinate_Y = 0;	
		draw_line( coordinate_X, coordinate_Y, adress);
		++coordinate_Y;
		++adress;
	}
}

void IS_KEY_PRESSED( byte2 X ){
	if( Key == V[X])
		PC += 2;
}

void IS_KEY_NOT_PRESSED( byte2 X ){
	if( Key != V[X])
		PC += 2;
}

void WAIT_FOR_KEY( byte2 X ){
	V[X] = Get_Key();
}

void SET_ADRESS_SPRITE( byte2 X ){
	I = V[X] * 5;	
}

void BINARY_CODED_DECIMAL( byte2 X ){
	byte remainder = 0;

	ram[I] = V[X] / 100;
	remainder = V[X] % 100;
	ram[I + 1] = remainder / 10;
	remainder = remainder % 10;
	ram[I + 2] = remainder;
}

void DUMP_REGISTERS( byte2 X ){
	for( byte j = 0; j <= X; ++j)
		ram[I + j] = V[j];	
}

void LOAD_REGISTERS( byte2 X ){
	for( byte j = 0; j <= X; ++j)
		V[j] = ram[I + j];	
}

void ADD_TO_ADRESS( byte2 X ){
	I += V[X];
}

void GET_DELAY( byte2 X ){
	V[X] = delay_timer;
}

void SET_DELAY( byte2 X){
	delay_timer = V[X];	
}

void SET_SOUND_TIMER( byte2 X ){
	sound_timer = V[X];
}
