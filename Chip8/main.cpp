//Chip-8 emulator
//(C) Adele Lynn Reed
//Done from scratch. Shoot me.

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Chip8CPU.h"

using namespace std;

int main()
{
	Chip8CPU* c8c = new Chip8CPU();

	c8c->loadInterpreter();
	c8c->loadROM();

	/*
	
	PAST THIS LINE IS OPENGL BULLSHITTERY-------------------------------------------------
	
	*/

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); //4x AA
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //OpenGL 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Open a window
	GLFWwindow* window = glfwCreateWindow(640, 320, "Chip-8 Emulator", NULL, NULL);
	
	//Check if it initialized correctly
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	//Make the window the current OGL context
	glfwMakeContextCurrent(window);
	glewExperimental = true; //To make GLFW happy.

	//Initializing GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW!");
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window))
	{
		//Draw shit.

		//Swap buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();

		//Rinse, repeat motherfucker.
	}
}