#pragma once

//Custom includes
#include "Model.h"
#include "Texture.h"

//Ball calss
class Ball : public Model
{
public:
	//Constructor
	Ball();

	//Variables
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 velocity;
	glm::vec3 colour;
	GLfloat radius;
	Texture texture;
};

