#pragma once
#include "Includes.h"

//Input processing class
class InputProcessor
{
public:
	//Constructor and destructor
	InputProcessor();
	~InputProcessor();

	//Update/process input
	void update();

	//Sets a key as being pressed or released
	void keyPressed(unsigned int keyID);
	void keyReleased(unsigned int keyID);

	//Resturns if a key is down or has been pressed
	bool isKeyDown(unsigned int keyID);
	bool isKeyPressed(unsigned int keyID);

	//Getters and setters for the mouse position 
	void mousePosition(float x, float y);
	glm::vec2 mousePosition(bool setPos, int screenWidth, int screenHeight);
	glm::vec2 mousePosition() const;

	//
	void mouseWheel(int wheelY);
	int mouseWheel() const;

private:
	//Returns if a key was previously down (last frame)
	bool wasKeyDown(unsigned int keyID);

	//Map of keys and previously pressed keys
	std::unordered_map<unsigned int, bool> keyMap;
	std::unordered_map<unsigned int, bool> previousKeyMap;

	//The mouse position
	glm::vec2 mousePos;
	glm::vec2 lastMousePos;

	//
	int wheelPos;
};