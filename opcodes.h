#include "prep.h"

void JMP( uint16_t address ){
	PC = address;
}
 
void CLEAR_DISPLAY(){
	for(uint8_t x = 0; x < MAX_WIDTH; ++x){
		for(uint8_t y = 0; y < MAX_HEIGHT; ++y)
			display[x][y] = OFF;
	}
	update_screen = 1;
}

void CALL_SUBROUTINE(uint16_t address){
	++stack_pointer;
	stack[stack_pointer] = PC;
	PC = address;
	
}

void RETURN_SUBROUTINE(){
	PC = stack[stack_pointer];
	--stack_pointer;
}

void IF_BYTE( uint16_t X, uint8_t byte){
	if(V[X] == byte)
		PC += 2; //talvez mudar dps	
}

void IF_NOT_BYTE( uint16_t X, uint8_t byte){
	if(V[X] != byte)
		PC += 2; //talvez mudar dps
}

void IF_VAR( uint16_t X, uint16_t Y){
	if(V[X] == V[Y])
		PC += 2; //talvez mudar dps
}

void IF_NOT_VAR( uint16_t X, uint16_t Y){
	if(V[X] != V[Y])
		PC += 2; //talvez mudar dps
}

void LOAD_BYTE( uint16_t X, uint8_t byte){
	V[X] = byte;
}

void LOAD_VAR( uint16_t X, uint16_t Y){
	V[X] = V[Y];
}

void ADD( uint16_t X, uint8_t byte){
	V[X] += byte;
}

void ADD_CARRY( uint16_t X, uint16_t Y){
	if( (V[X] + V[Y]) > 255)
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[X] += V[Y];
}

void OR( uint16_t X, uint16_t Y){
	V[X] |= V[Y];
}

void AND( uint16_t X, uint16_t Y){
	V[X] &= V[Y];
}

void XOR( uint16_t X, uint16_t Y){
	V[X] ^= V[Y];
}

void SUB_Y_FROM_X( uint16_t X, uint16_t Y){
	if(V[X] > V[Y])
		V[0xF] = 1;
	else
		V[0xF] = 0;	

	V[X] -= V[Y];
}

void SUB_X_FROM_Y( uint16_t X, uint16_t Y){
	if(V[Y] > V[X])
		V[0xF] = 1;
	else
		V[0xF] = 0;	

	V[X] = V[Y] - V[X];
}

void SHIFT_RIGHT( uint16_t X ){
	V[0xF] = Check_least_significant_bit( V[X] );
	V[X] >>= 1;	
}

void SHIFT_LEFT( uint16_t X ){
	V[0xF] = Check_most_significant_bit( V[X] );
	V[X] <<= 1;
}

void LOAD_ADDRESS( uint16_t address ){
	I = address;	
}

void JUMP2( uint16_t address){
	PC = V[0] + address;
}

void RAND( uint16_t X, uint8_t byte ){
	srand(time(0));
	uint8_t random = rand();
	//printf("random = %d\n", random);
	//printf("V[X] = %d\n", random & byte);
	V[X] = random & byte;
}

void DRAW( uint8_t X, uint8_t Y, uint16_t height, uint16_t address ){
	uint8_t sprite[height];	
	uint8_t area_of_display[15];
	uint8_t XOR_result[15];
	V[0xF] = 0;

	get_sprite(sprite, height, address);
	get_area_of_display(V[X], V[Y], area_of_display, height);
	XOR_sprite_area_of_display( sprite, area_of_display, XOR_result, height);

	if( collision_check( area_of_display, XOR_result, height) == TRUE)
		V[0xF] = 1;
	

	write_to_display( V[X], V[Y], XOR_result, height);
	update_screen = 1;
}



void IS_KEY_PRESSED( uint16_t X ){
	if( Key == V[X])
		PC += 2;
}

void IS_KEY_NOT_PRESSED( uint16_t X ){
	if( Key != V[X])
		PC += 2;
}

void WAIT_FOR_KEY( uint16_t X ){
	V[X] = Get_Key();
}

void SET_ADDRESS_SPRITE( uint16_t X ){
	I = V[X] * 5;	
}

void BINARY_CODED_DECIMAL( uint16_t X ){
	uint8_t remainder = 0;

	ram[I] = V[X] / 100;
	remainder = V[X] % 100;
	ram[I + 1] = remainder / 10;
	remainder = remainder % 10;
	ram[I + 2] = remainder;
}

void DUMP_REGISTERS( uint16_t X ){
	for( uint8_t j = 0; j <= X; ++j)
		ram[I + j] = V[j];	
}

void LOAD_REGISTERS( uint16_t X ){
	for( uint8_t j = 0; j <= X; ++j)
		V[j] = ram[I + j];	
}

void ADD_TO_ADDRESS( uint16_t X ){
	I += V[X];
}

void GET_DELAY( uint16_t X ){
	V[X] = delay_timer;
}

void SET_DELAY( uint16_t X){
	delay_timer = V[X];	
}

void SET_SOUND_TIMER( uint16_t X ){
	sound_timer = V[X];
}
