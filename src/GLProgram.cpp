#include "GLProgram.h"


//Constructor with initaliser list
GLProgram::GLProgram() : programID(0), vertexShaderID(0), fragmentShaderID(0) {}


//Destructor
GLProgram::~GLProgram() {}


//Initialisation of the vertex, fragment shaders and uniform attributes
void GLProgram::init(const std::string& vShaderPath, const std::string& fShaderPath)
{
	//Compiles the shaders, add the attributes and link the shaders
	compileShaders(vShaderPath, fShaderPath);
	linkShaders();
}


//Returns the program id
GLuint GLProgram::getProgramID() { return programID; }


//Uses the program object as part of current rendering state
void GLProgram::use()
{
	//Uses the program object as part of current rendering state
	glUseProgram(programID);
}


//Finishes using the program object as part of current rendering state
void GLProgram::unuse()
{
	//Finishes using the program object as part of current rendering state
	glUseProgram(0);
}


//Compile the vertex and fragment shaders
void GLProgram::compileShaders(const std::string& vShaderPath, const std::string& fShaderPath)
{
	//Assign the program object
	programID = glCreateProgram();

	//Assign the vertex shader object - if it fails display error and end application
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShaderID == 0)
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Vertex shader fail to be created!");

	//Assign the fragment shader object - if it fails display error and end application
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragmentShaderID == 0)
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Fragment shader fail to be created!");

	//Compile the vertex and fragment shaders
	compileShader(vShaderPath, vertexShaderID);
	compileShader(fShaderPath, fragmentShaderID);
}


//Compile a shader program - takes the filepath of the shader and the shader ID
void GLProgram::compileShader(const std::string& path, GLuint shaderID)
{
	//Opens the ifstream to read in the shader to
	//If it doesn't open report an error and end the applictaion
	std::ifstream shaderFile(path);
	if (shaderFile.fail())
	{
		perror(path.c_str());
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to open shader");
	}

	//Assign a string to read the shader program into
	//Read the shader in line by line into the fileContents string
	//Close the ifstream when finished
	std::string fileContents = "";
	std::string line;
	while (std::getline(shaderFile, line)) fileContents += line + "\n";
	shaderFile.close();

	//Compile the shader - assign a char pointer to the string containing the shader file
	const char* contentsPtr = fileContents.c_str();
	glShaderSource
	(
		shaderID,					//Handle of the shader object whose source code is to be replaced
		1,							//Number of elements in the string
		&contentsPtr,				//Array of pointers to string containing the source code to be loaded into the shader
		nullptr						//Specifies an array of string length
	);
	glCompileShader(shaderID);

	//Check to see if the shader compiled successfully
	GLint shaderCompiled = 0;
	glGetShaderiv
	(
		shaderID,					//Shader object to be queried
		GL_COMPILE_STATUS,			//The status of the shader - is it compiled?
		&shaderCompiled				//Returns the status of the shader compilation
	);

	//If the shader failed tp compile get the shader error
	if (shaderCompiled == GL_FALSE)
	{
		//Max length of the error log of the shader
		GLint infoLogLength = 0;
		glGetShaderiv
		(
			shaderID,				//Shader object to be queried
			GL_INFO_LOG_LENGTH,		//The GL info log length for the shader
			&infoLogLength			//Returns length of the shader info log
		);

		//Set up a vector of the size of the shader GL error log
		std::vector<char> errorLog(infoLogLength);
		glGetShaderInfoLog
		(
			shaderID,				//Shader object to be queried
			infoLogLength,			//The size of the character buffer for storing the returned information log
			&infoLogLength,			//The length of the string returned
			&errorLog[0]			//Return the information log of the shader
		);

		//Finally delete the shader - display the error and end the application
		glDeleteShader(shaderID);
		std::printf("%s\n", &(errorLog[0]));
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Shader failed to compile");
	}
}


//Link the vertex and fragment shaders to the program object
void GLProgram::linkShaders()
{
	//Link the shaders together into a program,
	//Attach the shaders to the program and link the program
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	//Check to see if the shader comiled successfully
	GLint isLinked = 0;
	glGetProgramiv
	(
		programID,					//The program object to be queried
		GL_LINK_STATUS,				//If the last link operation on program was successful will return true
		(int*)&isLinked				//Assign the success of the last link operation
	);

	//If the program did not link get the program error
	if (isLinked == GL_FALSE)
	{
		GLint infoLogLength = 0;
		glGetProgramiv
		(
			programID,				//The program object to be queried
			GL_INFO_LOG_LENGTH,		//The GL info log length for the program
			&infoLogLength			//Returns length of the program info log
		);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(infoLogLength);
		glGetProgramInfoLog
		(
			programID,				//The program object to be queried
			infoLogLength,			//The size of the character buffer for storing the returned information log
			&infoLogLength,			//The length of the string returned
			&errorLog[0]			//Return the information log of the program
		);

		//Delete the program and shaders
		glDeleteProgram(programID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		//Finally display the error and end the application
		std::printf("%s\n", &(errorLog[0]));
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Shaders failed to link");
	}

	//Always detach shaders after a successful link.
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	//Free shaders
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}