#include "chip8.h"

//Function to read the rom
void rom_reader(Chip8 *chip8, const char *rom_filename)
{
  long rom_length;
  uint8_t *rom_buffer;

  FILE *rom = fopen(rom_filename,"rb"); //Fetch the file.
  	if (rom != NULL)
    {
	   //Get the size of the file.
	   fseek(rom,0L,SEEK_END); 
	   rom_length = ftell(rom);
	   rewind(rom);
	   //Set buffer size to equal the size of the rom
	   rom_buffer = (uint8_t*) malloc(sizeof(uint8_t) * rom_length);

           if(rom_buffer == NULL)
	{
	   printf("ERROR: Out of memory\n");
	   exit(EXIT_FAILURE);
	}
	   //Read the file contents and save it into the buffer
	   fread(rom_buffer,sizeof(uint8_t),rom_length,rom);

	   if(PROGRAM_END_ADDR - PROGRAM_START_ADDR >= rom_length){
		for(int i = 0; i < rom_length; i++){
			chip8->ram[PROGRAM_START_ADDR + i] = rom_buffer[i];
		//	printf("0x%x\n",rom_buffer[i]); //For testing the buffer
		}
	   }
	   else{
		   printf("Rom file too large.\n");
		   exit(EXIT_FAILURE);
	   }
    } 
else
{
printf("ERROR: Rom file does not exist.\n");
exit(EXIT_FAILURE);
}

//CHIP-8 convention puts programs in memory at 0x200
//They will all have hardcoded addresses expecting that
//Read the file into memory at 0x200 and close it. 
unsigned char *buffer = malloc(rom_length+0x200); 
fclose(rom);
free(rom_buffer);
}

//Function to initialize the system
void init_system(Chip8 *chip8){

    chip8->is_running_flag = TRUE;
    chip8->draw_screen_flag = FALSE;
    chip8->is_paused_flag = FALSE;

    chip8->pc_reg = PC_START;
    chip8->current_op = 0;
    chip8->sp_reg = 0;
    chip8->i_reg = 0;

    // Clear display (memory)
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            chip8->screen[i][j] = 0;
        }
    }

    // Clear stack
    for (int i = 0; i < STACK_SIZE; i++) {
        chip8->stack[i] = 0;
    }

    // Clear ram
    for (int i = 0; i < TOTAL_RAM; i++) {
        chip8->ram[i] = 0;
	}
    // Clear registers
    for (int i = 0; i < NUM_V_REGISTERS; i++) {
        chip8->V[i] = 0;
    }

    // Load fontset into memory
    for(int i = 0; i < FONTSET_SIZE; i++) {
        chip8->ram[i] = FONTSET[i];
    }

    // Set timers to 0
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    // Keyboard setup
    for (int i = 0; i < NUM_KEYS; i++) {
        chip8->keyboard[i] = FALSE;
    }
    chip8->was_key_pressed = FALSE;
}

//Function to reset the system, same as Init but rom file is not cleared
//from ram 
void reset_system(Chip8 *chip8){
//Clear flags and set is-running flag to true
chip8->is_running_flag = TRUE;
chip8->draw_screen_flag = FALSE;
chip8->is_paused_flag = FALSE;
//Clear program counter, stack pointer, I register and current opcode
chip8->i_reg = 0;
chip8->pc_reg = PC_START;
chip8->sp_reg = 0;
chip8->current_op = 0;
//Clear screen 
for(int i = 0; i < SCREEN_WIDTH; i++){
	for(int j; j < SCREEN_HEIGHT; j++){
		chip8->screen[i][j] = 0;
	}

}
//Clear ram after fontset(80) values till 0x200
for(int i = 80; i < PROGRAM_START_ADDR; i++){
	chip8->ram[i] = 0;

}
//Clear V register,stack and keyboard
for(int i = 0; i < 16; i++){
	chip8->V[i] = 0;
	chip8->stack[i] = 0;
	chip8->keyboard[i] = FALSE;

}
//Clear all timers
chip8->delay_timer = 0;
chip8->sound_timer = 0;

}

//Fetches the next opcode, each opcode is 2 bytes long
uint16_t fetch_opcode(Chip8 *chip8){
	uint16_t opcode;
	uint8_t msB = chip8->ram[chip8->pc_reg];
	uint8_t lsB = chip8->ram[chip8->pc_reg + 1];
	opcode = msB << 8 | lsB;
	return opcode;
}

//Decodes the instructions that are fetched 
//If logging was enabled the program will print the opcode and what instruction was ran

void execute_instruction(Chip8 *chip8, int logging){
	uint16_t opcode = fetch_opcode(chip8);
	chip8->current_op = opcode;

	switch(opcode & 0xF000){
	case 0x0000:
		switch(opcode & 0x00FF){
		case 0x00E0:
		if(logging){printf("Instruction: Clear the screen (00E0)\n");}
		//cls(chip8);
		break;

		case 0x00EE:
		if(logging){printf("Instruction: Return from subroutine(00EE)\n");}
		//return_from_subroutine(chip8);
		break;

		default:
		printf("ERROR: Instruction not recognized 0x%X\n", opcode);
		exit(EXIT_FAILURE);
		}
		break;

	case 0x1000:
		if(logging){printf("Instruction: Jump to address NNN (1NNN)\n");}
		//jump(chip8);
		break;

	case 0x2000:
		if(logging){printf("Instruction: Call subroutine at address NNN (2NNN)\n");}
		//call_subroutine(chip8); 
		break;

	case 0x3000:
		if(logging){printf("Instruction: Skip next instruction if VX == NN (3XNN)\n");}
		//se_Vx_nn(chip8); 
		break;

	case 0x4000:
		if(logging){printf("Instruction: Skip next instruction if VX != NN (4XNN)\n");}
		//sne_Vx_nn(chip8);
		break;	

	case 0x5000:
		if(logging){printf("Instruction: Skips the next instruction if VX == VY (5XY0)\n");}
		//se_Vx_Vy(chip8);
		break;
	
	case 0x6000:
		if(logging){printf("Instruction: Sets VX to NN (6XNN)\n");}
		//set_Vx_nn(chip8);
		break;

	case 0x7000:
		if(logging){printf("Instruction: Adds NN to VX (7XNN)\n");}
		//add_nn_Vx(chip8);
		break;
	
	case 0x8000:
		switch(opcode & 0x000F){
			case 0x0000:
				if(logging){printf("Instruction: Set VX to VY (8XY0)\n");}
				//set_Vx_Vy(chip8);
				break;

			case 0x0001:
				if(logging){printf("Instruction: Set VX to (VX OR VY)(Bitwise OR operation) (8XY1)\n");}
				//set_Vx_Vx_OR_Vy(chip8);
				break;

			case 0x0002:
				if(logging){printf("Instruction: Set VX to (VX AND VY)(Bitwise AND operation) (8XY2)\n");}
			       	//set_Vx_Vx_AND_Vy(chip8);
				break;

			case 0x0003:
				if(logging){printf("Instruction: Set VX to (VX XOR VY)(Bitwise XOR operation) (8XY3)\n");}
				//set_Vx_Vx_XOR_Vy(chip8);
				break;

			case 0x0004:
				if(logging){printf("Instruction: Add VY to VX (8XY4)\n");}
				//add_Vy_Vx(chip8);
				break;

			case 0x0005:
				if(logging){printf("Instruction: Subtract VY from VX (8XY5)\n");}
				//sub_Vy_Vx(chip8);
				break;

			case 0x0006:
				if(logging){printf("Instruction: Store the least significant bit of VX in VF and then shifts VX to the right by 1 (8XY6)\n");}
				//shr(chip8);
				break;

			case 0x0007:
				if(logging){printf("Set VX to (VY-VX) (8XY7)\n");}
				//set_Vx_Vy_sub_Vx(chip8);
				break;

			case 0x000E:
				if(logging){printf("Stores the most significant bit of VX in VF and then shifts VX to the left by 1 (8XYE)\n");}
				//shl(chip8);
				break;

			default:
				printf("ERROR: Unrecognized opcode 0x%X\n",opcode);
				exit(EXIT_FAILURE);



		}
		break;

	case 0x9000:
		if(logging){printf("Instruction: Skip next instruction if VX != VY (9XY0)\n");}
		//sne_Vx_Vy(chip8);
		break;

	case 0xA000:
		if(logging){printf("Instruction: Set I to address NNN (ANNN)\n");}
		//set_I_nnn(chip8);
		break;

	case 0xB000:
		if(logging){printf("Instruction: Jump to address NNN + V0 (BNNN)\n");}
		//jmp_V0(chip8);
		break;

	case 0xC000:
		if(logging){printf("Instruction: Set VX = rand()&NN (CXNN)\n");}
		//rand_Vx(chip8);
		break;

	case 0xD000:
		if(logging){printf("Instruciton: Draw at coordinate(VX,VY,N) (DXYN)\n");}
		//draw(chip8);
		break;

	case 0xE000:
		switch(opcode & 0x00FF){
			case 0x009E:
				if(logging){printf("Instruction: Skip instruction if key() == VX (EX9E)\n");}
				//ske_Vx(chip8);
				break;

			case 0x00A1:
				if(logging){printf("Instruction: Skip instruction if key() != VX (EXA1)\n");}
				//skne_Vx(chip8);
				break;

			default:
				printf("ERROR: Unrecognized opcode 0x%X\n",opcode);
				exit(EXIT_FAILURE);
		
		
	}
		break;

	case 0xF000:
		switch(opcode & 0x00FF){

			case 0x0007:
				if(logging){printf("Instruction: Set VX to the value of the delay timer (FX07)\n");}
				//set_Vx_dt(chip8);
				break;

			case 0x000A:
				if(logging){printf("Instruction: A key press is awaited, and then stored in VX (FX0A)\n");}
				//set_Vx_key(chip8);
				break;

			case 0x0015:
				if(logging){printf("Instruction: Set delay timer to the value of VX (FX15)\n");}
				//set_dt_Vx(chip8);
				break;

			case 0x0018:
				if(logging){printf("Instruction: Set sound timer to the value of VX (FX18)\n");}
				//set_st_Vx(chip8);
				break;

			case 0x001E:
				if(logging){printf("Instruction: Adds VX to I (FX1E)\n");}
				//add_Vx_I(chip8);
				break;

			case 0x0029:
				if(logging){printf("Instruction: Sets I to the font character in VX (FX29)\n");}
				//ld_F_Vx(chip8);
				break;

			case 0x0033:
				if(logging){printf("Instruction: Store BCD of VX value (FX33)\n");}
				//str_bcd_Vx(chip8);
				break;

			case 0x0055:
				if(logging){printf("Instruction: Store registers V[0] - V[X] starting at I register address (FX55)\n");}
				//str_regs_Vx(chip8);
				break;

			case 0x0065:
				if(logging){printf("Instruction: Fill registers V[0] - V[X] from values starting at I register address (FX65)\n");}
				//fill_regs_Vx(chip8);
				break;

			default:
				printf("ERROR: Unrecognized opcode 0x%X\n", opcode);
				exit(EXIT_FAILURE);




		}
		
		break;

	default:
		printf("ERROR: Unrecognized opcode 0x%X\n",opcode);
		exit(EXIT_FAILURE);

		}

	


	}

void process_user_input(Chip8 *chip8){

SDL_Event e;

while(SDL_PollEvent(&e)){

	if(e.type == SDL_KEYDOWN){

		switch(e.key.keysym.sym){

			case SDLK_ESCAPE:
				chip8->is_running_flag = FALSE;
				break;

			case SDLK_SPACE:
				if(chip8->is_paused_flag){
					chip8->is_paused_flag = FALSE;

				}

				else{
					chip8->is_paused_flag = TRUE;

				}
				break;

			case SDLK_F5:
				reset_system(chip8);
				break;

			default:
				break;
	}
		for(int i = 0; i < NUM_KEYS; i++){
			if(e.key.keysym.sym = KEYMAP[i]){
				chip8->keyboard[i] = TRUE;

			}

		}

	}

		if(e.type == SDL_KEYUP){
			for(int i = 0; i < NUM_KEYS; i++){
				if(e.key.keysym.sym = KEYMAP[i]){
					chip8->keyboard[i] = FALSE;
				}

			}

		}

		if(e.type == SDL_QUIT){
			chip8->is_running_flag = FALSE;

		}
	}
}




void update_timers(Chip8 *chip8){
	if(chip8->delay_timer > 0){
		chip8->delay_timer--;
	}
	if(chip8->sound_timer > 0){
		chip8->sound_timer--;
	}
}


void print_regs(Chip8 *chip8){
	printf("Current OP: 0x%x\n", chip8->current_op);
	for(int i = 0; i < NUM_V_REGISTERS; i++){
		printf("V Register %x: 0x%x\n", i, chip8->V[i]);
	}
	printf("Index Reg: 0x%x\n", chip8->i_reg);
	printf("PC Reg: 0x%x\n", chip8->pc_reg);
	printf("SP Reg: 0x%x\n", chip8->sp_reg);
	printf("Delay Timer Reg: 0x%x\n", chip8->delay_timer);
	printf("Sound Timer Reg: 0x%x\n", chip8->sound_timer);
	printf("\n");
}

void print_stack(Chip8 *chip8){
	for(int i = 0; i < STACK_SIZE; i++){
		printf("Stack Element %i: 0x%x\n", i, chip8->stack[i]);
	}
}


void print_keyboard(Chip8 *chip8){
	for(int i = 0; i < NUM_KEYS; i++){
		printf("Keyboard Key %x: %i\n", i, chip8->keyboard[i]);
	}

}
