#pragma once
#include "Includes.h"

//Square struct
class Square
{
public:
	//Constructor
	Square();

	//Set the buffers of the square
	void setBuffers();

	//Render function
	void render();

	//Defines vertices for a square
	GLfloat vertices[20] = 
	{
		 //Vertices					Texture coords
		 1.0f,  1.0f, 0.0f,			1.0f, 0.0f,		//Top right
		 1.0f,  0.0f, 0.0f,			1.0f, 1.0f,		//Bottom right
		 0.0f,  0.0f, 0.0f,			0.0f, 1.0f,		//Bottom left
		 0.0f,  1.0f, 0.0f,			0.0f, 0.0f		//Top left 	
	};

	//Define indices for a square
	GLuint indices[6] =
	{
		0, 1, 3,
		1, 2, 3
	};

	//Vertex buffer object
	GLuint VBO;

	//Vertex array object
	GLuint VAO;

	//Element (index) buffer object
	GLuint EBO;
};