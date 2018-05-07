#pragma once
#include "Includes.h"

//Texture class
class Texture
{
public:
	//Load textures from file
	void load(const std::string fileName);

	//Set buffers 
	void setBuffers();

	//Bind the texture
	void bind() const;
	
	//SDL surface and texture handle
	SDL_Surface* tex;
	GLuint texture;
};