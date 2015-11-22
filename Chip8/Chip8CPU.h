#pragma once

class Chip8CPU
{
public:
	Chip8CPU();
	~Chip8CPU();

	void CPUCycle();
	bool loadROM(char input[]);
	void loadInterpreter();

	int screen[64 * 32];

	//Screen size 64x32

	unsigned char mem[4096];
	//Chip 8 memory:
	/*
	4Kb

	start:
	0x000

	0x000-0x1FF reserved for interpreter.

	0x200 start of most chip-8 programs

	0x600 start of most ETI 660 Chip-8 programs.

	0xFFF end of RAM
	*/

	int delay = 0;
	int sound = 0;

	//Stack, stack pointer, program counter, index, opcode.

	unsigned short stack[16], sp, pc, I, opcode;
};

