#pragma once

//Custom includes
#include "Includes.h"
#include "Vertex.h"

//Model class
class Model
{
public:
	//Load a model using assimp
	void loadASSIMP(std::string const& path);

	//Render model
	void render();

	//Set the model buffers
	void setBuffers();

	//Model data - position/normals/texture coordinates/indices
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

private:
	//Vertex buffer object
	GLuint VBO;

	//Vertex array object
	GLuint VAO;

	//Element (index) buffer object
	GLuint EBO;
};