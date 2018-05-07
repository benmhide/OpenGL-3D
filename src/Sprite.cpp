#include "Sprite.h"

//Constructor
Sprite::Sprite()
{
	//Defualt values
	position = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f, 1.0f, 0.0f };
	colour = { 1.0f, 1.0f, 1.0f };
	active = true;
}