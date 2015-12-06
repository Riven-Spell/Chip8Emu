#include "Chip8CPU.h"
#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

Chip8CPU::Chip8CPU()
{
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;
	for (int i = 0; i < (64 * 32); i++)
	{
		screen[i] = 0x0;
	}
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

	opcode = mem[pc] << 8 | mem[pc+1];

	bool input[16];

	input[0] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_1);
	input[1] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_2);
	input[2] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_3);
	input[3] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_4);

	input[4] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q);
	input[5] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W);
	input[6] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E);
	input[7] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R);

	input[8] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A);
	input[9] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S);
	input[10] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D);
	input[11] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_F);

	input[12] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Z);
	input[13] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_X);
	input[14] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_C);
	input[15] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_V);

	if (!skipForFx0A)
	{
		switch (opcode & 0xF000)
		{
		case 0x0000:
			switch (opcode & 0x0F00)
			{
			case 0x0000:
				switch (opcode && 0x00FF)
				{
				case 0x00E0:
					//Clear screen
					for (int i : screen)
					{
						i = 0;
					}
					break;
				case 0x00EE:
					//Return from subroutine
					pc = stack[sp];
					sp--;
					break;
				}
				break;
			}
			break;
		case 0x1000:
			//Jump to nnn
			break;
		case 0x2000:
			//Call subroutine at nnn
			pc = opcode & 0x0FFF;
			break;
		case 0x3000:
			//Skip next instruction if Vx = kk
			if (V[(opcode & 0x0F00) >> 8] == opcode & 0x00FF)
			{
				pc += 2;
			}
			break;
		case 0x4000:
			//Skip next if Vx != kk
			if (V[(opcode & 0x0F00) >> 8] != opcode & 0x00FF)
			{
				pc += 2;
			}
			break;
		case 0x5000:
			//skip next if Vx = Vy
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
			{
				pc += 2;
			}
			break;
		case 0x6000:
			//Set Vx to kk
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			break;
		case 0x7000:
			//Set Vx to Vx + kk
			V[(opcode & 0x0F00) >> 8] = mem[(opcode & 0x0F00) >> 8] + opcode & 0x00FF;
			break;
		case 0x8000:
			switch (opcode & 0x000F)
			{
			case 0x0000:
				//set Vx to Vy
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
				break;
			case 0x0001:
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
				break;
			case 0x0002:
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
				break;
			case 0x0003:
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
				break;
			case 0x0004:
				if (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 256)
				{
					V[15] = 1;
				}
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
				break;
			case 0x0005:
				if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
				{
					V[15] = 1;
				}
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
				break;
			case 0x0006:
				V[15] = V[(opcode & 0x0F00) >> 8] & 0x1;
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] / 2;
				break;
			case 0x0007:
				if (V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4])
				{
					V[15] = 1;
				}
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
				break;
			case 0x000E:
				V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
				V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] / 2;
				break;
			}
			break;
		case 0x9000:
			if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
			{
				pc += 2;
			}
			break;
		case 0xA000:
			I = 0x0FFF;
			break;
		case 0xB000:
			pc = 0x0FFF + V[0x0];
			break;
		case 0xC000:
			srand(time(NULL));
			V[(opcode & 0x0F00) >> 8] = (rand() % 255) & (opcode & 0x00FF);
			break;
		case 0xD000:
			//Draw sprite!
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short height = opcode & 0x000F;
			unsigned short pixel;

			V[0xF] = 0;
			for (int yline = 0; yline < height; yline++)
			{
				pixel = mem[I + yline];
				for (int xline = 0; xline < 8; xline++)
				{
					if ((pixel & (0x80 >> xline)) != 0)
					{
						if (screen[(x + xline + ((y + yline) * 64))] == 1)
							V[0xF] = 1;
						screen[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}
			break;
		case 0xE000:
			switch (opcode && 0x00FF)
			{
			case 0x009E:
				if (input[V[(opcode & 0x0F00) >> 8]] == true)
				{
					pc += 2;
				}
				break;
			case 0x00A1:
				if (input[V[(opcode & 0x0F00) >> 8]] == false)
				{
					pc += 2;
				}
				break;
			}
			break;
		case 0xF000:
			switch (opcode & 0x00FF)
			{
			case 0x0007:
				V[(opcode & 0x0F00) >> 8] = delay;
				break;
			case 0x000A:
				skipForFx0A = true;
				storeInFx0A = (opcode & 0x0F00) >> 8;
				break;
			case 0x0015:
				delay = V[(opcode & 0x0F00) >> 8];
				break;
			case 0x0018:
				sound = V[(opcode & 0x0F00) >> 8];
				break;
			case 0x001E:
				I = V[(opcode & 0x0F00) >> 8] + I;
				break;
			case 0x0029:
				I = mem[0x80 + (V[(opcode & 0x0F00) >> 8] * 5)];
				break;
			case 0x0033:
				mem[I] = V[(opcode & 0x0F00) >> 8] / 100;
				mem[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
				mem[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
				break;
			case 0x0055:
				for (int i = I; i < 15; i++)
				{
					mem[i] = V[i - I];
				}
				break;
			case 0x0065:
				for (int i = I; i < 15; i++)
				{
					V[i-I] = mem[i];
				}
				break;
			}
			break;
		default:
			printf("Invalid opcode 0x%X\n", opcode);
			break;
		}
		pc += 2;
	}
	else
	{
		for (int i = 0x0; i < 15; i++)
		{
			if (input[i])
			{
				skipForFx0A = false;
				V[storeInFx0A] = i;
			}
			i += 0x1;
		}
	}
	
}

void Chip8CPU::loadInterpreter()
{
	//It'd be better to store this stuff in-program.
	char fset[80] =
	{
		0xF0,0x90,0x90,0x90,0xF0, //0
		0x20,0x60,0x20,0x20,0x70, //1
		0xF0,0x10,0xF0,0x80,0xF0, //2
		0xF0,0x10,0xF0,0x10,0xF0, //3
		0x90,0x90,0xF0,0x10,0x10, //4
		0xF0,0x80,0xF0,0x10,0xF0, //5
		0xF0,0x80,0xF0,0x90,0xF0, //6
		0xF0,0x10,0x20,0x40,0x40, //7
		0xF0,0x90,0xF0,0x90,0xF0, //8
		0xF0,0x90,0xF0,0x10,0xF0, //9
		0xF0,0x90,0xF0,0x90,0x90, //A
		0xE0,0x90,0xE0,0x90,0xE0, //B
		0xF0,0x80,0x80,0x80,0xF0, //C
		0xE0,0x90,0x90,0x90,0xE0, //D
		0xF0,0x80,0xF0,0x80,0xF0, //E
		0xF0,0x80,0xF0,0x80,0x80 //F
	};
	int c = 80;
	for (char f:fset)
	{
		mem[c] = fset[c-80];
		c++;
	}
}

bool Chip8CPU::loadROM(char input[])
{
	int i = 0;
	string n = "";
	while (i < sizeof(input))
	{
		n += input[i];
		i++;
	}

	ifstream myfile;
	myfile.open(n);
	string o;
	if (myfile.is_open())
	{
		int c = 512;
		while (getline(myfile, o))
		{
			i = 0;
			while (i < o.size())
			{
				mem[c] = o.c_str()[i];
				c++;
				i++;
			}
		}
		return true;
	}
	else
	{
		fprintf(stderr, "Couldn't read specified file. \n");
		return false;
	}
}

Chip8CPU::~Chip8CPU()
{
}
