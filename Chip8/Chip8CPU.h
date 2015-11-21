#pragma once

class Chip8CPU
{
public:
	Chip8CPU();
	~Chip8CPU();

	void CPUCycle();
	void loadROM(char input[]);
	void loadInterpreter();

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


};

