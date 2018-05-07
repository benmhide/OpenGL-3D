#include "Ball.h"


//Constructor
Ball::Ball()
{
	position = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f, 1.0f, 0.0f };
	velocity = { 0.0f, 0.0f, 0.0f };
	colour = { 1.0f, 1.0f, 1.0f };
	radius = 1.0f;
}