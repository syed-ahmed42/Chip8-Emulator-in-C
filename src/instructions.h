#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "chip8_t.h"

long random_at_most(long max); //random number generator in a range

//Instructions listed in the order of their opcodes

void cls(Chip8 *chip8); //00E0
void return_from_subroutine(Chip8 *chip8); //00EE
void jump(Chip8 *chip8); //1NNN
void call_subroutine(Chip8 *chip8); //2NNN
void se_Vx_nn(Chip8 *chip8); //3XNN
void sne_Vx_nn(Chip8 *chip8); //4XNN
void se_Vx_Vy(Chip8 *chip8); //5XY0
void set_Vx_nn(Chip8 *chip8); //6XNN
void add_nn_Vx(Chip8 *chip8); //7XNN
void set_Vx_Vy(Chip8 *chip8); //8XY0
void set_Vx_Vx_OR_Vy(Chip8 *chip8); //8XY1
void set_Vx_Vx_AND_Vy(Chip8 *chip8); //8XY2
void set_Vx_Vx_XOR_Vy(Chip8 *chip8); //8XY3
void add_Vy_Vx(Chip8 *chip8); //8XY4
void sub_Vy_Vx(Chip8 *chip8); //8XY5
void shr(Chip8 *chip8); //8XY6
void set_Vx_Vy_sub_Vx(Chip8 *chip8); //8XY7
void shl(Chip8 *chip8); //8XYE
void sne(Chip8 *chip8); //9XY0
void set_I_nnn(Chip8 *chip8); //ANNN
void jmp_V0(Chip8 *chip8); //BNNN
void rand_Vx(Chip8 *chip8); //CXNN
void draw(Chip8 *chip8); //DXYN
void ske_Vx(Chip8 *chip8); //EX9E
void skne_Vx(Chip8 *chip8); //EXA1
void set_Vx_dt(Chip8 *chip8); //FX07
void set_Vx_key(Chip8 *chip8); //FX0A
void set_dt_Vx(Chip8 *chip8); //FX15
void set_st_Vx(Chip8 *chip8); //FX18
void add_Vx_I(Chip8 *chip8); //FX1E
void ld_F_Vx(Chip8 *chip8); //FX29
void str_bcd_Vx(Chip8 *chip8); //FX33
void str_regs_Vx(Chip8 *chip8); //FX55
void fill_regs_Vx(Chip8 *chip8); //FX65


#endif
