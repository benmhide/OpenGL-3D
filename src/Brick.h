#pragma once

//Custom includes
#include "Model.h"
#include "Texture.h"

//Brick class - inherits from square
class Brick : public Model
{
public:
	//Constructor
	Brick();

	//Brick member variables for:
	//position, scale, colour and rotation
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 colour;
	GLfloat rotation;

	//Brick texture
	Texture texture;
	Texture cracked;

	//Number of brick hits
	GLint hits;

	//Booleans for brick
	bool brickAlive;
	bool brickDying;
};