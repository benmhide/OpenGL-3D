#pragma once
#include "Includes.h"

//Shader program class
class GLProgram
{
public:
	//Constructor/Destructor
	GLProgram();
	~GLProgram();

	//Initialiser function
	void init(const std::string& vShaderPath, const std::string& fShaderPath);

	//Returns the program id
	GLuint getProgramID();

	//Use and unuse the program
	void use();
	void unuse();

private:
	//Functions to compile, add uniform attributes
	//and link the shader programs to the program object
	void compileShaders(const std::string& vShaderPath, const std::string& fShaderPath);
	void compileShader(const std::string& path, GLuint shaderID);
	void linkShaders();

	//Program vertex shader and fragment shader object handles
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
};