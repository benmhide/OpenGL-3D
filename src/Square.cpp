#include "Square.h"


//Constructor
Square::Square() {}


//Set the buffers of the square
void Square::setBuffers()
{
	//OpenGL buffers
	//Set up 1 for the triangle
	glGenBuffers(1, &VBO);

	//Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
	glGenVertexArrays(1, &VAO);

	//Initialise the index buffer
	glGenBuffers(1, &EBO);

	//Bind Vertex Array Object
	glBindVertexArray(VAO);

	//Copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Set up the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Set the colour attribute pointer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Unbind the VAO
	glBindVertexArray(0);
}


//Render function
void Square::render()
{
	//Render the square 
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}