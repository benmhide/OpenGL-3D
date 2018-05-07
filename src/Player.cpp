#include "Player.h"


//Constructor
Player::Player()
{
	//Defualt values
	position = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f, 1.0f, 0.0f };
	colour = { 1.0f, 1.0f, 1.0f };
	rotation = 0.0f;
	lives = 3;
	score = 0;
}