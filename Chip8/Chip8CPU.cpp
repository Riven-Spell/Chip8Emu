#include "Chip8CPU.h"
#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>
#include <GLFW\glfw3.h>
using namespace std;

Chip8CPU::Chip8CPU()
{
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;
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

	opcode = mem[pc];

	bool input[4][4];

	input[0][0] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_1);
	input[0][1] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_2);
	input[0][2] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_3);
	input[0][3] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_4);

	input[1][0] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q);
	input[1][1] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W);
	input[1][2] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E);
	input[1][3] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R);

	input[1][0] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A);
	input[1][1] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S);
	input[1][2] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D);
	input[1][3] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_F);

	input[1][0] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Z);
	input[1][1] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_X);
	input[1][2] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_C);
	input[1][3] = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_V);

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
		
		break;
	case 0x4000:
		//Skip next if Vx != kk
		
		break;
	case 0x5000:
		//skip next if Vx = Vy
		
		break;
	case 0x6000:
		//Set Vx to kk
		mem[opcode & 0x0F] = opcode & 0x00FF;
		break;
	case 0x7000:
		//Set Vx to Vx + kk
		mem[opcode & 0x0F] = mem[opcode & 0x0f] + opcode & 0x00FF;
		break;
	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0001:
			break;
		case 0x0002:
			break;
		case 0x0003:
			break;
		case 0x0004:
			break;
		case 0x0005:
			break;
		case 0x0006:
			break;
		case 0x0007:
			break;
		case 0x000E:
			break;
		}
		break;
	case 0x9000:
		break;
	case 0xA000:
		break;
	case 0xC000:
		break;
	case 0xD000:
		//Draw sprite!
		break;
	case 0xE000:
		switch (opcode && 0x00FF)
		{
		case 0x009E:
			break;
		case 0x00A1:
			break;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:
			break;
		case 0x000A:
			break;
		case 0x0015:
			break;
		case 0x0018:
			break;
		case 0x001E:
			break;
		case 0x0029:
			break;
		case 0x0033:
			break;
		case 0x0055:
			break;
		case 0x0065:
			break;
		}
		break;
	default:
		printf("Invalid opcode ", opcode);
		break;
	}
	pc++;
}

void Chip8CPU::loadInterpreter()
{
	//Char sprites stored from 0x000 to 0x1FF
	ifstream myfile;
	myfile.open("fontset.txt");
	string i;
	if (myfile.is_open())
	{
		int c = 80;
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
			//SHIIIT way to load-- fix this.
			mem[c] = o.c_str()[0];
			c++;
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
