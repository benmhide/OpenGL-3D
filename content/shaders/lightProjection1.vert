#version 440 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;

out vec3 fragmentPosition;
out vec3 fragmentColor;
out vec2 texture;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat uRotation;

void main()
{
	gl_Position = uProjection * uView * uModel * uRotation * vec4(position, 1.0);
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}

#version 330
in vec3 position;
in vec3 vertexColor;
in vec2 texture;

out vec3 fragmentPosition;
out vec3 fragmentColor;
out vec2 Texture;

uniform mat4 modelMatrix      = mat4(1.0);
uniform mat4 viewMatrix       = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 rotateMatrix = mat4(1.0);

void main()
{
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * rotateMatrix * vec4(position, 1.0);
		fragmentColor =  mat3(transpose(inverse(modelMatrix))) * vertexColor; 
		fragmentPosition = vec3(modelMatrix * rotateMatrix * vec4(position, 1.0f));
		Texture = texture;
}