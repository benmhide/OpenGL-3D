#include "Texture.h"


//Set the buffers
void Texture::setBuffers()
{
	//Generate and bind the textures
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);

	//Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Generate mipmaps - generate them automatically
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(tex);
}

//Bind the texture
void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

//Load the texture
void Texture::load(const std::string fileName)
{
	//Use SDL image loading function in this case
	tex = IMG_Load(fileName.c_str());

	if (tex == NULL)
	{
		perror(fileName.c_str());
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Error loading texture");
	}
}