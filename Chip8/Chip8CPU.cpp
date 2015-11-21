#include "Chip8CPU.h"
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

Chip8CPU::Chip8CPU()
{

}

void Chip8CPU::CPUCycle()
{
	/*
	
	Chip 8 has 16 key hexidecimal keypad with following layout
	123C
	456D
	789E
	A0BF

	remap to:

	1234
	QWER
	ASDF
	ZXCV

	*/

	//bool input[4][4];
}

void Chip8CPU::loadInterpreter()
{
	//Char sprites stored from 0x000 to 0x1FF
	ifstream myfile;
	myfile.open("fontset.txt");
	string i;
	if (myfile.is_open())
	{
		int c = 0;
		while (getline(myfile,i))
		{
			mem[c] = i.c_str()[0];
			c++;
		}
	}
	else
	{
		fprintf(stderr, "Couldn't read fontset.txt \n");
	}
}

void Chip8CPU::loadROM()
{

}

Chip8CPU::~Chip8CPU()
{
}
