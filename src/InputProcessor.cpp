#include "InputProcessor.h"


//Constructor 
InputProcessor::InputProcessor() : mousePos(0), lastMousePos(0), wheelPos(0) {}


//Destructor
InputProcessor::~InputProcessor() {}


//Update (process input)
void InputProcessor::update()
{
	//Store the previous key presses
	for (auto& it : keyMap) previousKeyMap[it.first] = it.second;

	//
	wheelPos = 0;
}


//Sets a key being pressed as true
void InputProcessor::keyPressed(unsigned int keyID) { keyMap[keyID] = true; }


//Sets a key which is released as not pressed (false)
void InputProcessor::keyReleased(unsigned int keyID) { keyMap[keyID] = false; }


//Returns if a key is pressed down
bool InputProcessor::isKeyDown(unsigned int keyID)
{
	//Loop through to find the key
	auto it = keyMap.find(keyID);

	//Return true if key is pressed else return false
	if (it != keyMap.end()) return it->second;
	else return false;
}


//Returns if a key has been pressed
bool InputProcessor::isKeyPressed(unsigned int keyID)
{
	//If the key is down and wasn't down the last update
	if (isKeyDown(keyID) && !wasKeyDown(keyID))	return true;
	return false;
}


//Set the mouse position
void InputProcessor::mousePosition(float x, float y)
{
	//Set the mouse positions
	lastMousePos = mousePos;
	mousePos.x = x;
	mousePos.y = y;
}


//Set the mouse position
glm::vec2 InputProcessor::mousePosition(bool setPos, int screenWidth, int screenHeight)
{
	//The x and y offsest
	float xoffset, yoffset;

	//if updating the mouse position
	if (setPos)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		xoffset = (float)(x - (screenWidth / 2));
		yoffset = (float)(y - (screenHeight / 2));
		lastMousePos = mousePos;
	}

	//Record the last mouse position
	lastMousePos = mousePos;

	//Return the mouse position
	return glm::vec2(xoffset, yoffset);
}


//Get the mouse position
glm::vec2 InputProcessor::mousePosition() const { return mousePos; }


//
void InputProcessor::mouseWheel(int wheelY)
{
	wheelPos = wheelY;
}


//
int InputProcessor::mouseWheel() const
{
	return wheelPos;
}



//Returns the previous state of the key presse
bool InputProcessor::wasKeyDown(unsigned int keyID)
{
	//Loop through to find the keys previous state
	auto it = previousKeyMap.find(keyID);

	//Return true if key was pressed else return false
	if (it != previousKeyMap.end()) return it->second;
	else return false;
}