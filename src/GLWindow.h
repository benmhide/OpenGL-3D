#pragma once
#include "Includes.h"

//GLWindow class
class GLWindow
{
public:
	//Constructor/Destructor
	GLWindow();
	~GLWindow();

	//Initialisation and OpenGL rendering window update function
	void init(std::string name, int &w, int &h);
	void swapBuffer();
	SDL_Window* window();

	//Screen dimensions acessors
	int screenWidth();
	int screenHeight();

private:
	//Clean function
	void clean();

	//SDL window and SDL_GLContext
	SDL_Window* win;
	SDL_GLContext context;

	//Game screen dimensions
	int height;
	int width;
};