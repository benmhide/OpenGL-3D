#version 440 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoordinates;
layout (location = 2) in vec3 inNormals;

out vec2 textureCoordinates;
out vec3 transposedNormals;
out vec3 fragmentPosition;
out vec3 lightColour;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uLightColour;

void main()
{   
	gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0f);
	
	fragmentPosition = vec3(uModel * vec4(inPosition, 1.0f));
	
	transposedNormals = mat3(transpose(inverse(uModel))) * inNormals;  
	
	textureCoordinates = inTextureCoordinates;
	
	lightColour = uLightColour;
}