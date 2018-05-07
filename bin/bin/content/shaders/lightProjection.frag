#version 440 core
out vec4 colourFragment;

in vec2 textureCoordinates;
in vec3 transposedNormals;
in vec3 fragmentPosition;
in vec3 lightColour;

uniform vec3 uLightPosition; 
uniform vec3 uViewPosition; 
uniform vec3 uObjectColour;
uniform sampler2D uTexture;

void main()
{
    //Ambient light component
    float ambientCoefficient = 0.5f;
    vec3 ambient = ambientCoefficient * lightColour;
  	
    //Diffuse light component
	float diffuseCoefficient = 0.8f;	
    vec3 normalizedNormal = normalize(transposedNormals);
    vec3 lightDirection = normalize(uLightPosition - fragmentPosition);
    float diffuseMax = max(dot(normalizedNormal, lightDirection), 0.0);
    vec3 diffuse = diffuseCoefficient * diffuseMax * lightColour;
	
	//Specular light component
    float specularCoefficient = 0.7f;
    vec3 viewDirection = normalize(uViewPosition - fragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);
	float specularMax = pow(max(dot(viewDirection, reflectDirection), 0.0), 128.0);
    vec3 specular = specularCoefficient * specularMax * lightColour;  
    
	//Resultant output fragment colour
	vec3 ambientDiffuseSpecular = (ambient + diffuse + specular) * uObjectColour;
    colourFragment =  vec4(ambientDiffuseSpecular, 1.0) * texture(uTexture, textureCoordinates);
} 


	