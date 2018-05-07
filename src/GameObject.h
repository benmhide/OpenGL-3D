#pragma once

//Custom includes
#include "Model.h"
#include "Texture.h"

//GameObject class - inherits from square
class GameObject : public Model
{
public:
	//Constructor
	GameObject();

	//Brick member variables for:
	//position, scale, colour and rotation
	glm::vec3 position;
	glm::vec3 scale;
	GLfloat rotation;
	glm::vec3 colour;

	//Texture 
	Texture texture;

	//Is the gamobject active
	bool active;
};