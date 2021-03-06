#include "instructions.h"





void cls(Chip8 *chip8){ //00E0
	for(int i = 0; i < SCREEN_HEIGHT; i++){
		for(int j = 0; j < SCREEN_WIDTH; j++){
			chip8->screen[i][j] = 0;
		}
	}	

	chip8->draw_screen_flag = 1;
	chip8->pc_reg += 2;
}	




void return_from_subroutine(Chip8 *chip8){ //00EE
	chip8->sp_reg--;
	chip8->pc_reg = chip8->stack[chip8->sp_reg];
	chip8->pc_reg += 2;
}




void jump(Chip8 *chip8){ //1NNN
	uint16_t nnn = chip8->current_op & 0x0FFF;

	chip8->pc_reg = nnn;
}




void call_subroutine(Chip8 *chip8){ //2NNN
	uint16_t nnn = chip8->current_op & 0x0FFF;	 

	chip8->stack[chip8->sp_reg] = chip8->pc_reg;
	chip8->sp_reg++; 
	chip8->pc_reg = nnn; 
}




void se_Vx_nn(Chip8 *chip8){ //3XNN
	uint8_t nn = chip8->current_op & 0x00FF;
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;

	if(chip8->V[target_vx_reg] == nn)
	{
		chip8->pc_reg += 4;
	}	
	else
	{
		chip8->pc_reg += 2;
	}
}




void sne_Vx_nn(Chip8 *chip8){ //4XNN
	uint8_t nn = chip8->current_op & 0x00FF;
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;

	if(chip8->V[target_vx_reg] != nn)
	{
		chip8->pc_reg += 4;
	}	
	else
	{
		chip8->pc_reg += 2;
	}
}




void se_Vx_Vy(Chip8 *chip8){ //5XY0
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t target_vy_reg = (chip8->current_op) >> 4;

	if(chip8->V[target_vx_reg] == chip8->V[target_vy_reg])
	{
		chip8->pc_reg += 4;
	}
	else
	{
		chip8->pc_reg += 2;
	}
}




void set_Vx_nn(Chip8 *chip8){ //6XNN
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t nn = chip8->current_op & 0x00FF;

	chip8->V[target_vx_reg] = nn;
	chip8->pc_reg += 2;
	
}




void add_nn_Vx(Chip8 *chip8){ //7XNN
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t nn = chip8->current_op & 0x00FF;

	chip8->V[target_vx_reg] += nn;
	chip8->pc_reg += 2;
}




void set_Vx_Vy(Chip8 *chip8){ //8XY0
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t target_vy_reg = (chip8->current_op & 0x00F0) >> 4;
	
	chip8->V[target_vx_reg] = chip8->V[target_vy_reg];
      	chip8->pc_reg += 2;
}




void set_Vx_Vx_OR_Vy(Chip8 *chip8){ //8XY1
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t target_vy_reg = (chip8->current_op & 0x00F0) >> 4;

	chip8->V[target_vx_reg] = (chip8->V[target_vx_reg] | chip8->V[target_vy_reg]);
	chip8->pc_reg += 2;
}




void set_Vx_Vx_AND_Vy(Chip8 *chip8){ //8XY2
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t target_vy_reg = (chip8->current_op & 0x00F0) >> 4;

	chip8->V[target_vx_reg] = (chip8->V[target_vx_reg] & chip8->V[target_vy_reg]);
	chip8->pc_reg += 2;
}




void set_Vx_Vx_XOR_Vy(Chip8 *chip8){ //8XY3
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t target_vy_reg = (chip8->current_op & 0x00F0) >> 4;

	chip8->V[target_vx_reg] = (chip8->V[target_vx_reg] ^ chip8->V[target_vy_reg]);
	chip8->pc_reg += 2;
}




void add_Vy_Vx(Chip8 *chip8){ //8XY4
	uint8_t target_vx_reg = (chip8->current_op * 0x0F00) >> 8;
	uint8_t target_vy_reg = (chip8->current_op * 0x00F0) >> 4;
	uint16_t sum = (chip8->V[target_vx_reg] + chip8->V[target_vy_reg]);

	if(sum > 0x00FF)
	{
		chip8->V[0xF] = 1;
	}
	else
	{
		chip8->V[0xF] = 0;
	}
	
	chip8->V[target_vx_reg] = (sum * 0xFF);
	chip8->pc_reg += 2;
}




void sub_Vy_Vx(Chip8 *chip8){ //8XY5
	uint8_t target_vx_reg = (chip8->current_op * 0x0F00) >> 8;
	uint8_t target_vy_reg = (chip8->current_op * 0x00F0) >> 4;

	if(chip8->V[target_vx_reg] > chip8->V[target_vy_reg])
	{
		chip8->V[0xF] = 1;
	}
	else
	{
		chip8->V[0xF] = 0;
	}
	
	chip8->V[target_vx_reg] -= chip8->V[target_vy_reg];
	chip8->pc_reg += 2;
}




void shr(Chip8 *chip8){ //8XY6
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t lsB = (chip8->V[target_vx_reg] % 2);

	if(lsB == 1)
	{
		chip8->V[0xF] = 1;
	}
	else
	{
		chip8->V[0xF] = 0;
	}

	chip8->V[target_vx_reg] = chip8->V[target_vx_reg] >> 1; 
	chip8->pc_reg += 2;
}




void set_Vx_Vy_sub_Vx(Chip8 *chip8){ //8XY7
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t target_vy_reg = (chip8->current_op & 0x00F0) >> 4;

	if(chip8->V[target_vy_reg] > chip8->V[target_vx_reg])
	{
		chip8->V[0xF] = 1;
	}
	else
	{
		chip8->V[0xF] = 0;
	}
	
	chip8->V[target_vx_reg] = chip8->V[target_vy_reg] - chip8->V[target_vx_reg];
	chip8->pc_reg += 2;
}	




void shl(Chip8 *chip8){ //8XYE
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t target_vy_reg = (chip8->current_op & 0x00F0) >> 4;
	uint8_t msB = chip8->V[target_vx_reg] >> 7;

	if(msB == 1)
	{
		chip8->V[0xF] = 1;
	}
	else
	{
		chip8->V[0xF] = 0;
	}

	chip8->V[target_vx_reg] *= 2;
	chip8->pc_reg += 2;
}




void sne(Chip8 *chip8){ //9XY0
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t target_vy_reg = (chip8->current_op & 0x00F0) >> 4;

	if(chip8->V[target_vx_reg] != chip8->V[target_vy_reg])
	{
		chip8->pc_reg += 4;
	}
	else
	{
		chip8->pc_reg += 2;
	}
}




void set_I_nnn(Chip8 *chip8){ //ANNN
	uint16_t nnn = chip8->current_op & 0x0FFF;

	chip8->i_reg = nnn; 
	chip8->pc_reg += 2;
}




void jmp_V0(Chip8 *chip8){ //BNNN
	uint16_t nnn = chip8->current_op & 0x0FFF; 
	
	chip8->pc_reg = (nnn + chip8->V[0]);	
}




void rand_Vx(Chip8 *chip8){ //CXNN
/*	uint8_t nn = (chip8->current_op & 0x00FF);
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t random_number = random_at_most(255);

	chip8->V[target_vx_reg] = (random_number & nn);
	chip8->pc_reg += 2;    */
    uint8_t target_v_reg = (chip8->current_op & 0x0F00) >> 8;
    uint8_t kk = chip8->current_op & 0x00FF;
    uint8_t random_num = rand() % 256;

    chip8->V[target_v_reg] = random_num & kk;
    chip8->pc_reg += 2;
}




void draw(Chip8 *chip8){ //DXYN
//uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
//uint8_t target_vy_reg = (chip8->current_op & 0x00F0) >> 4;
//uint8_t n_byte = chip8->current_op & 0x000F;
//uint8_t xor_value;
//uint8_t temp;
//
//for(int i = 0; i < n_byte + 1; i++)
//{
//	for(int j = 0; j < 8; j++)
//	{
//		uint8_t dividend = 128;
//		uint8_t shift = 7;
//		uint8_t bit = chip8->i_reg;
//		uint8_t temp = bit & dividend;
//		uint8_t shifted_value = bit >> shift;
//		xor_value = (chip8->screen[i][j] ^ shifted_value);
//
//		if(xor_value == 0)
//		{
//			chip8->V[0xF] = 1;
//		}
//		else
//		{
//			chip8->V[0xF] = 0;
//		}
//		dividend /= 2;
//		shift--;
//	}
//
//	chip8->i_reg += 1;
//}
//
//chip8->pc_reg += 2;
    uint8_t target_v_reg_x = (chip8->current_op & 0x0F00) >> 8;
    uint8_t target_v_reg_y = (chip8->current_op & 0x00F0) >> 4;
    uint8_t sprite_height = chip8->current_op & 0x000F;
    uint8_t x_location = chip8->V[target_v_reg_x];
    uint8_t y_location = chip8->V[target_v_reg_y];
    uint8_t pixel;

    // Reset collision register to FALSE
    chip8->V[0xF] = 0;
    for (int y_coordinate = 0; y_coordinate < sprite_height; y_coordinate++) {
        pixel = chip8->ram[chip8->i_reg + y_coordinate];
        for (int x_coordinate = 0; x_coordinate < 8; x_coordinate++) {
            if ((pixel & (0x80 >> x_coordinate)) != 0) {
                if (chip8->screen[y_location + y_coordinate][x_location + x_coordinate] == 1) {
                    chip8->V[0xF] = 1;
                }
                chip8->screen[y_location + y_coordinate][x_location + x_coordinate] ^= 1;
            }
        }
    }

    chip8->draw_screen_flag = 1;
    chip8->pc_reg += 2;
}	




void ske_Vx(Chip8 *chip8){ //EX9E
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t vx_value = chip8->V[target_vx_reg];

		if(chip8->keyboard[vx_value] == 1)
		{
			chip8->pc_reg += 4;
		}
		else
		{
			chip8->pc_reg += 2;
		}
}




void skne_Vx(Chip8 *chip8){ //EXA1
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	uint8_t vx_value = chip8->V[target_vx_reg];
	
	if(chip8->keyboard[vx_value] != 1)
	{
		chip8->pc_reg += 4;
	}
	else
	{
		chip8->pc_reg += 2;
	}
}




void set_Vx_dt(Chip8 *chip8){ //FX07
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;

	chip8->V[target_vx_reg] = chip8->delay_timer;	
	chip8->pc_reg += 2;
}




void set_Vx_key(Chip8 *chip8){ //FX0A
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	
	chip8->was_key_pressed = 0;

	for(int i = 0; i < NUM_KEYS; i++)
	{
		if(chip8->keyboard[i] == 1)
		{
			chip8->was_key_pressed = 1;
			chip8->V[target_vx_reg] = i;
		}
	}	

	if(!chip8->was_key_pressed)
	{
		return;
	}

	chip8->pc_reg += 2;
}




void set_dt_Vx(Chip8 *chip8){ //FX15
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;

	chip8->delay_timer = chip8->V[target_vx_reg];
	chip8->pc_reg += 2;

}




void set_st_Vx(Chip8 *chip8){ //FX18
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;

	chip8->sound_timer = chip8->V[target_vx_reg];
	chip8->pc_reg += 2;
}




void add_Vx_I(Chip8 *chip8){ //FX1E
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;

	chip8->i_reg += chip8->V[target_vx_reg];
	chip8->pc_reg += 2;	
}




void ld_F_Vx(Chip8 *chip8){ //FX29
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;

	chip8->i_reg = (chip8->V[target_vx_reg] * 0x5);
	chip8->pc_reg += 2;
}




void str_bcd_Vx(Chip8 *chip8){ //FX33
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;
	
	uint8_t last_digit = (chip8->V[target_vx_reg] % 10);
	uint8_t middle_digit = ((chip8->V[target_vx_reg] / 10) % 10);
	uint8_t first_digit = ((chip8->V[target_vx_reg] / 100) % 10);

	chip8->ram[chip8->i_reg] = first_digit;
	chip8->ram[chip8->i_reg + 1] = middle_digit;
	chip8->ram[chip8->i_reg + 2] = last_digit;

	chip8->pc_reg += 2;
}	




void str_regs_Vx(Chip8 *chip8){ //FX55
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;

	for(int i = 0; i < target_vx_reg; i++)
	{
		chip8->ram[chip8->i_reg + i] = chip8->V[i];
	}
	
	chip8->pc_reg += 2;
}




void fill_regs_Vx(Chip8 *chip8){ //FX65
	uint8_t target_vx_reg = (chip8->current_op & 0x0F00) >> 8;

	for(int i = 0; i < target_vx_reg; i++)
	{
		 chip8->V[i] = chip8->ram[chip8->i_reg + i];
	}
	
	chip8->pc_reg += 2;
}




//Calculate unbiased random number
/*long random_at_most(long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = random();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
}*/
