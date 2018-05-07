#pragma once

//Custom includes
#include "Square.h"
#include "Texture.h"

//GameObject class - inherits from square
class Sprite : public Square
{
public:
	//Constructor
	Sprite();

	//Brick member variables for:
	//position, scale, colour and rotation
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 colour;

	//Texture 
	Texture texture;

	//Is the gamobject active
	bool active;
};