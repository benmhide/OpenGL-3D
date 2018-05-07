#version 440 core
layout (location = 0) in vec3 Position; //vertex positions
layout (location = 1) in vec2 texCoord;	//texture coordinates

out vec2 textureCoordinate;

//Uniform for model, view and projection matrices
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	//the clip-space output position of the current vertex
	gl_Position = uProjection * uView * uModel * vec4(Position, 1.0f);	
	
	//Assign the texCoord to the textureCoordinate to pass to the fragment shader
	textureCoordinate = vec2(texCoord.x, 1 - texCoord.y);	
}