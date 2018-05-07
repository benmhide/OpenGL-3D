#include "Includes.h"
#include "Player.h"
#include "Ball.h"
#include "Square.h"
#include "Brick.h"
#include "GameObject.h"
#include "Texture.h"
#include "GLWindow.h"
#include "GLProgram.h"
#include "InputProcessor.h"
#include "Sprite.h"

#pragma region Variables and Objects
//Game states
enum class GameState { Play, Win, Lose, Exit };
GameState state;

//Game elements
GLWindow window;
GLProgram glProgram;
GLProgram glSpriteProgram;
InputProcessor ip;

//Camera
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 25.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

//Camera views
glm::vec3 ballView = cameraPosition;
glm::vec3 playerView = cameraPosition;
glm::vec3 camView1 = glm::vec3(0.0f, -15.0f, 15.0f);
glm::vec3 camView2 = glm::vec3(0.0f, 15.0f, 20.0f);
glm::vec3 camView3 = glm::vec3(25.0f, 0.0f, 20.0f);;
glm::vec3 camView5 = cameraPosition;

//Camera variables
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat FOV = 45.0f;
GLfloat mouseSensitivity = 1.0f;
GLfloat cameraSpeed = 5.0f;
GLboolean updateView;
GLboolean freeCamera;
GLboolean followPlayer;

//DeltaTime
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Light
glm::vec3 lightPosition(30.0f, 30.0f, 30.0f);
glm::vec3 lightColour(0.8f, 0.9f, 0.8f);
GLfloat lightRotation = -0.001f;

//Screen dimensions
GLint screenWidth;
GLint screenHeight;
GLfloat aspectRatio;

//Tranform matrices 
glm::mat4 modelTranslate;
glm::mat4 modelScale;
glm::mat4 modelRotation;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 orthoProgMatrix;
glm::mat4 orthoViewMatrix;

//Ball
Ball ball;
glm::vec3 ballVelocity = { 5.5f, 10.0f, 0.0f };
bool stuckToPaddle;
GLfloat offset = 0;

//Player
Player player;
glm::vec3 playerVelocity = { 15.0f, 0.0f, 0.0f };
bool moveLeft;
bool moveRight;
int score;

//Bricks
const GLint numbBricksHigh = 5;
const GLint numbBricksWide = 10;
const GLint boundBlocks = 20;
const GLint topBlocks = 25;
Brick bricks[numbBricksHigh][numbBricksWide];
Brick boundLeft[boundBlocks];
Brick boundRight[boundBlocks];
Brick boundTop[topBlocks];

//Game objects
Sprite lives;
Sprite win;
Sprite gameover;
GameObject skybox;
GameObject powerUp;
std::vector<Texture> scoreText;
std::vector<Sprite> scoreObject;

//Variables
int powerUps;
bool gameWon;
bool fullscreen;
#pragma endregion

#pragma region Functions
//Game functions
void init();
void buildLevel();
void loadScore();
void setScore();
void gameLoop();
bool wonGame();

//Power-up functions
void setPowerUp();
void powerUpActive(int x, int y);
void maxHits();

//Brick functions
void setCrackedBrick(int x, int y);
void setDeadBrick(int x, int y);
void setDyingBrick(int x, int y);

//Collision functions
bool collisionDetection(Ball &ball, Brick &brick);
bool collisionDetection(Ball &ball, Player &player);
bool collisionDetection(GameObject &powerUp, Player &player);

//Input function
void input();

//Update functions
void update();
void updateBall();
void updatePlayer();

//Render functions
void startRender();
void render();
void resetMatrices();
void renderObject(glm::mat4 translation, glm::mat4 rotation, glm::mat4 scale, glm::vec3 colour, Texture &texture);
void renderSprite(glm::mat4 translation, glm::mat4 scale, glm::vec3 colour, Texture &texture);
void endRender();

//Aspect ratio and fullscreen
void maintainAspectRatio();
bool toggleFullScreen();

//Camera controls
void mouseInput(float xOffset, float yOffset, bool constrainPitch);
void updateCamera();
void updateCameraView(int view);
#pragma endregion

//Main program
int main(int argc, char* argv[])
{
	//Initialisation
	init();

	//Game loop
	gameLoop();

	//End game
	return 0;
}

//Initialisation
void init()
{
	//Window
	window.init("Benjamin Hide // HID16605093 // Graphics // CGP2012M-1718", screenWidth, screenHeight);

	//Game state
	state = GameState::Play;
	bool gameWon = false;
	powerUps = 0;
	fullscreen = false;;

	//Camera bools
	updateView = false;
	freeCamera = false;
	followPlayer = false;

	//Player score
	score = 0;

	//Ball stuck to paddle
	stuckToPaddle = true;

	//Player controls
	moveLeft = false;
	moveRight = false;

	//Initialise shaders
	glProgram.init("..//content//shaders//lightProjection.vert", "..//content//shaders//lightProjection.frag");
	glSpriteProgram.init("..//content//shaders//projection.vert", "..//content//shaders//projection.frag");

	//Set aspect ratio
	aspectRatio = (GLfloat)screenHeight / (GLfloat)screenWidth;

	//Orthographic projection and view matrices
	orthoProgMatrix = glm::ortho(0.0f, (GLfloat)screenWidth, (GLfloat)screenHeight, 0.0f, -1.0f, 1.0f);
	orthoViewMatrix = glm::mat4(1.0f);

	//Use the shader program
	glProgram.use();

	//Skybox
	{
		//Set player
		skybox.loadASSIMP("..//content//models//cube1.obj");
		skybox.setBuffers();
		skybox.scale = glm::vec3(100.0f);
		skybox.position = glm::vec3(0.0f);

		//Player texture
		skybox.texture.load("..//content//skycube.png");
		skybox.texture.setBuffers();
	}

	//Player
	{
		//Set player
		player.loadASSIMP("..//content//models//cube1.obj");
		player.setBuffers();
		player.scale = glm::vec3(1.5f, 0.125f, 0.5f);
		player.position = glm::vec3
		(
			0.0f,
			-7.5f,
			0.0f
		);

		//Player texture
		player.texture.load("..//content//player.png");
		player.texture.setBuffers();
		player.colour = { 1.0f, 1.0f, 0.0f };
		offset = player.scale.x;
	}

	//Ball
	{
		//Set ball
		ball.loadASSIMP("..//content//models//sphere1.obj");
		ball.setBuffers();
		ball.scale = glm::vec3(0.1f, 0.1f, 0.1f);
		ball.radius = ball.scale.x / 2;
		ball.position = glm::vec3
		(
			player.position.x,
			player.position.y + /*(player.scale.y * 2) +*/ ball.scale.y,
			player.position.z
		);

		//Ball texture
		ball.texture.load("..//content//newball.png");
		ball.texture.setBuffers();
	}

	//Level
	{
		//Set level
		buildLevel();
	}

	//Power up
	{
		//Set player
		powerUp.loadASSIMP("..//content//models//cube1.obj");
		powerUp.setBuffers();
		powerUp.scale = glm::vec3(0.5f, 0.5f, 0.5f);
	
		//Player texture
		powerUp.texture.load("..//content//powerUp.png");
		powerUp.texture.setBuffers();
		powerUp.active = false;
	}

	//Lives
	{
		//Set lives
		lives.setBuffers();
		lives.scale = glm::vec3(30.0f, 30.0f, 1.0f);
		lives.position = glm::vec3
		(
			30.0f,
			(GLfloat)screenHeight - lives.scale.y - 5.0f,
			0.0f
		);

		//Lives texture
		lives.texture.load("..//content//heart.png");
		lives.texture.setBuffers();
	}

	//Win and gameover
	{
		//Set win
		win.setBuffers();
		win.scale = glm::vec3(screenWidth / 2, screenHeight / 2, 1.0f);
		win.position = glm::vec3
		(
			screenWidth / 4,
			screenHeight / 4,
			0.0f
		);

		//Win texture
		win.texture.load("..//content//youWin.png");
		win.texture.setBuffers();
		win.active = false;
	}
	{
		//Set gameover
		gameover.setBuffers();
		gameover.scale = glm::vec3(screenWidth / 2, screenHeight / 2, 1.0f);
		gameover.position = glm::vec3
		(
			screenWidth / 4,
			screenHeight / 4,
			0.0f
		);

		//Lives texture
		gameover.texture.load("..//content//gameOver.png");
		gameover.texture.setBuffers();
		gameover.active = false;
	}

	//Score
	{
		loadScore();
	}
}

//Render the level
void buildLevel()
{
	//Brick texture
	Texture texture;
	Texture cracked;
	texture.load("..//content//block.png");
	texture.setBuffers();
	cracked.load("..//content//crackedBlock.png");
	cracked.setBuffers();

	//Position and scale of the bricks
	for (int y = 0; y < numbBricksHigh; y++)
	{
		for (int x = 0; x < numbBricksWide; x++)
		{
			//Brick
			Brick brick;

			//Set brick
			brick.loadASSIMP("..//content//models//cube1.obj");
			brick.setBuffers();
			brick.scale = glm::vec3(0.5f, 0.5f, 0.5f);
			brick.position = (glm::vec3(-9.0f + (2.0f * x), (2.0f * y), 0.0f));

			//Set brick colours
			if (y == 0 || y == 5)	   brick.colour = { 1.0f, 0.5f, 0.0f };
			else if (y == 1 || y == 6) brick.colour = { 0.5f, 1.0f, 0.5f };
			else if (y == 2 || y == 7) brick.colour = { 0.0f, 0.5f, 1.0f };
			else if (y == 3 || y == 8) brick.colour = { 1.0f, 1.0f, 0.0f };
			else if (y == 4 || y == 9) brick.colour = { 1.0f, 0.0f, 0.0f };
			else					   brick.colour = { 1.0f, 1.0f, 1.0f };

			//Brick textures
			brick.texture = texture;
			brick.cracked = cracked;

			//Set the bricks
			bricks[y][x] = brick;
		}
	}

	//Left Bound
	for (int i = 0; i < boundBlocks; i++)
	{
		//Brick
		Brick brick;

		//Set brick
		brick.loadASSIMP("..//content//models//cube1.obj");
		brick.setBuffers();
		brick.scale = glm::vec3(0.5f, 0.5f, 0.5f);
		brick.position = (glm::vec3(-12.0f, -10.0f + (1.0f * i), 0.0f));

		//Set brick colours
		brick.colour = { 1.0f, 1.0f, 0.0f };

		//Brick textures
		brick.texture = texture;

		//Set the bricks
		boundLeft[i] = brick;
	}

	//Right Bound
	for (int i = 0; i < boundBlocks; i++)
	{
		//Brick
		Brick brick;

		//Set brick
		brick.loadASSIMP("..//content//models//cube1.obj");
		brick.setBuffers();
		brick.scale = glm::vec3(0.5f, 0.5f, 0.5f);
		brick.position = (glm::vec3(12.0f, -10.0f + (1.0f * i), 0.0f));

		//Set brick colours
		brick.colour = { 1.0f, 1.0f, 0.0f };

		//Brick textures
		brick.texture = texture;

		//Set the bricks
		boundRight[i] = brick;
	}

	//Top Bound
	for (int i = 0; i < topBlocks; i++)
	{
		//Brick
		Brick brick;

		//Set brick
		brick.loadASSIMP("..//content//models//cube1.obj");
		brick.setBuffers();
		brick.scale = glm::vec3(0.5f, 0.5f, 0.5f);
		brick.position = (glm::vec3(-12.0f + (1.0f * i), 10.0f, 0.0f));

		//Set brick colours
		brick.colour = { 1.0f, 1.0f, 0.0f };

		//Brick textures
		brick.texture = texture;

		//Set the bricks
		boundTop[i] = brick;
	}
}



//Game Loop
void gameLoop()
{
	//Game Loop
	//While game is not in the exit state
	while (state != GameState::Exit)
	{
		//DeltaTime
		float currentFrame = (float)SDL_GetTicks();
		deltaTime = (float)(currentFrame - lastFrame) / 1000.0f;
		lastFrame = currentFrame;

		//Handle input
		input();

		//Handle updates
		update();

		//Handle rendering
		render();
	}
}

//Input handler function
void input()
{
	//SDL event
	SDL_Event events;

	//While there are events to process
	while (SDL_PollEvent(&events))
	{
		//Switch on SDL event type
		switch (events.type)
		{
		case SDL_QUIT: state = GameState::Exit; break;
		case SDL_KEYDOWN: ip.keyPressed(events.key.keysym.sym); break;
		case SDL_KEYUP:	ip.keyReleased(events.key.keysym.sym); break;
		case SDL_MOUSEBUTTONDOWN: ip.keyPressed(events.button.button); break;
		case SDL_MOUSEBUTTONUP: ip.keyReleased(events.button.button); break;
		case SDL_MOUSEMOTION: ip.mousePosition((float)events.motion.x, (float)events.motion.y); break;
		case SDL_MOUSEWHEEL: ip.mouseWheel(events.wheel.y); break;
		case SDL_WINDOWEVENT: if (events.window.event == SDL_WINDOWEVENT_RESIZED) maintainAspectRatio(); break;
		}
	}
}

//Game update handler function
void update()
{
	//Set the camera view
	int cam = 0;
	if (ip.isKeyDown(SDLK_1)) {	cam = 1; updateView = true; }
	if (ip.isKeyDown(SDLK_2)) { cam = 2; updateView = true; }
	if (ip.isKeyDown(SDLK_3)) { cam = 3; updateView = true; }
	if (ip.isKeyDown(SDLK_4)) { cam = 4; updateView = true; }
	if (ip.isKeyDown(SDLK_5)) { cam = 5; updateView = true; }

	//If free moving camera is enabled
	if (freeCamera)
	{
		//Hide the cusor
		SDL_ShowCursor(0);

		//Warp the mouse to the center of the screen (cusor hidden)
		SDL_WarpMouseInWindow(window.window(), screenWidth / 2, screenHeight / 2);

		//Move the camera (UP/DOWN/LEFT/RIGHT/BACKWARDS/FORWARDS)
		if (ip.isKeyDown(SDLK_w)) cameraPosition += cameraSpeed * cameraFront * deltaTime;
		if (ip.isKeyDown(SDLK_s)) cameraPosition -= cameraSpeed * cameraFront * deltaTime;
		if (ip.isKeyDown(SDLK_a)) cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
		if (ip.isKeyDown(SDLK_d)) cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
		if (ip.isKeyDown(SDLK_q)) cameraPosition[1] += cameraSpeed * deltaTime;
		if (ip.isKeyDown(SDLK_e)) cameraPosition[1] -= cameraSpeed * deltaTime;

		//Move the camera on mouse control
		if (ip.isKeyDown(SDL_BUTTON_RIGHT))
		{
			//Get the mouse position
			glm::vec2 mousePos = ip.mousePosition(true, screenWidth, screenHeight);
			mouseInput(mousePos.x, mousePos.y, true);
		}

		//Modify the FOV (camera zoom)
		if (ip.mouseWheel() != 0)
		{
			//Set the FOV on mouse wheel motion
			if (FOV >= 1.0f && FOV <= 45.0f) FOV += ip.mouseWheel();
			if (FOV <= 1.0f) FOV = 1.0f;
			if (FOV >= 45.0f) FOV = 45.0f;
		}
	}

	//Show the cusor
	else SDL_ShowCursor(1);

	//Toggle fullscreen and quit the game
	if (ip.isKeyDown(SDLK_ESCAPE)) state = GameState::Exit;
	if (ip.isKeyPressed(SDLK_F1)) fullscreen = toggleFullScreen();

	//Update the inputProcessor
	ip.update();

	//Update the camera
	updateCamera();

	//Update camera view
	if (updateView) updateCameraView(cam);

	//Finite state machine - play state
	if (state == GameState::Play)
	{
		//Is the game won
		gameWon = wonGame();

		//If the game is won change to winning state
		if (gameWon) state = GameState::Win;

		//Update the pladdle
		updatePlayer();

		//Update the ball
		updateBall();
	}

	//Win state
	else if (state == GameState::Win) win.active = true;

	//Lose state
	else if (state == GameState::Lose) gameover.active = true;
}

//Update the paddle
void updatePlayer()
{
	//Release the ball
	if (ip.isKeyDown(SDLK_SPACE)) stuckToPaddle = false;

	//Player is alive
	if (player.lives > 0)
	{
		//Move the paddle left
		if (ip.isKeyDown(SDLK_LEFT))
		{
			//If player not at the edge of screen
			if (player.position.x > -11.25f + offset)
			{
				player.position.x -= playerVelocity.x * deltaTime;
			}
		}

		//Move the paddle right
		if (ip.isKeyDown(SDLK_RIGHT))
		{
			//If player not at the edge of screen
			if (player.position.x < 11.15f - offset)
			{
				player.position.x += playerVelocity.x * deltaTime;
			}
		}

		//Ball stuck to paddle
		if (stuckToPaddle)
		{
			//Set the balls position
			ball.position = glm::vec3
			(
				player.position.x,
				player.position.y + player.scale.y + (ball.scale.y * 2),
				player.position.z
			);
		}
	}

	//Player is dead
	else if (player.lives <= 0)	state = GameState::Lose;

	//Use for testing - delete when finished
	if (!stuckToPaddle)	player.position.x = ball.position.x;

	//Player follow view
	if (followPlayer) cameraPosition = glm::vec3(player.position.x, -12.0f, player.position.z + cameraPosition.z);
}

//Update the ball
void updateBall()
{
	//Ball stuck to paddle
	if (!stuckToPaddle)
	{
		//Move the ball in x
		ball.position.x += ballVelocity.x * deltaTime;

		//Keep ball inside the bounds of the window - left side
		if (ball.position.x <= -11.0f)
		{
			ballVelocity.x = -ballVelocity.x;
			ball.position.x = -11.0f;
		}

		//Right side
		else if (ball.position.x >= 11.0f)
		{
			ballVelocity.x = -ballVelocity.x;
			ball.position.x = 11.0f;
		}

		//Check each brick for collision on the left and right
		for (int y = 0; y < numbBricksHigh; y++)
		{
			for (int x = 0; x < numbBricksWide; x++)
			{
				//If the brick is active
				if (bricks[y][x].brickAlive)
				{
					//If a collision occurs between the ball and brick
					if (collisionDetection(ball, bricks[y][x]))
					{
						setCrackedBrick(x, y);
						ballVelocity.x = -ballVelocity.x;
						ball.position.x += ballVelocity.x * deltaTime;
					}
					if (bricks[y][x].hits < 0) setDeadBrick(x, y);
				}
				//Brick is dying
				if (bricks[y][x].brickDying) setDyingBrick(x, y);

				//Brick is gone
				if (bricks[y][x].position.y < -15.0f) bricks[y][x].brickDying = false;
			}
		}

		//Move the ball in y
		ball.position.y += ballVelocity.y * deltaTime;


		//Top side
		if (ball.position.y >= 9.0f)
		{
			ballVelocity.y = -ballVelocity.y;
			ball.position.y = 9.0f;
		}

		//Bottom side
		else if (ball.position.y <= -15.0f)
		{
			player.lives--;
			stuckToPaddle = true;
		}

		//Player side - avoid ball sticking in paddle
		else if (collisionDetection(ball, player)) ballVelocity.y = 1 * abs(ballVelocity.y);
		
		//Check each brick for collision on the top and bottom
		for (int y = 0; y < numbBricksHigh; y++)
		{
			for (int x = 0; x < numbBricksWide; x++)
			{
				//If the brick is active
				if (bricks[y][x].brickAlive)
				{
					//If a collision occurs between the ball and brick
					if (collisionDetection(ball, bricks[y][x]))
					{
						setCrackedBrick(x, y);
						ballVelocity.y = -ballVelocity.y;
						ball.position.y += ballVelocity.y * deltaTime;
					}
					if (bricks[y][x].hits < 0) setDeadBrick(x, y);
				}
				//Brick is dying
				if (bricks[y][x].brickDying) setDyingBrick(x, y);
		
				//Brick is gone
				if (bricks[y][x].position.y < -15.0f) bricks[y][x].brickDying = false;
			}
		}
		
		//If the power up is active
		powerUpActive(2, 4);
		powerUpActive(4, 3);
		powerUpActive(6, 2);
		
		//Move the power up
		if (powerUp.active)
		{
			//Move the power up down
			powerUp.position.y -= 7.5f * deltaTime;
		
			//Player collects power up 
			if (collisionDetection(powerUp, player))
			{
				powerUp.active = false;
				powerUp.position.y = 15.0f;
				powerUps++;
				setPowerUp();
			}
		}
		
		//Power up lost
		if (powerUp.position.y < -15.0f)
		{
			powerUp.active = false;
			powerUp.position.y = 15.0f;
		}
	}
}

//Start the rendering process
void startRender()
{
	//Start the rendering process
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Use the shader program
	glProgram.use();
}

//Render the scene
void render()
{
	//Start rendering
	startRender();

	//Move light source
	glm::mat4 rotationMat(1);
	rotationMat = glm::rotate(rotationMat, lightRotation, glm::vec3(1.0f, 1.0f, 0.0f));
	lightPosition = glm::vec3(rotationMat * glm::vec4(lightPosition, 1.0));

	//SkyBox
	{
		resetMatrices();
		modelTranslate = glm::translate(modelTranslate, skybox.position);
		modelScale = glm::scale(modelScale, skybox.scale);
		modelRotation = glm::rotate(modelRotation, skybox.rotation += deltaTime / 8, glm::vec3(0.0f, 1.0f, 0.0f));
		renderObject(modelTranslate, modelRotation, modelScale, skybox.colour, skybox.texture);
		skybox.render();
	}

	//Render the level
	{
		for (int y = 0; y < numbBricksHigh; y++)
		{
			for (int x = 0; x < numbBricksWide; x++)
			{
				resetMatrices();
				modelTranslate = glm::translate(modelTranslate, bricks[y][x].position);
				modelScale = glm::scale(modelScale, bricks[y][x].scale);
				modelRotation = glm::rotate(modelRotation, bricks[y][x].rotation += deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
				renderObject(modelTranslate, modelRotation, modelScale, bricks[y][x].colour, bricks[y][x].texture);
				bricks[y][x].render();
			}
		}
	}

	//Render player
	{
		resetMatrices();
		modelTranslate = glm::translate(modelTranslate, player.position);
		modelScale = glm::scale(modelScale, player.scale);
		modelRotation = glm::rotate(modelRotation, player.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		renderObject(modelTranslate, modelRotation, modelScale, player.colour, player.texture);
		player.render();
	}

	//Render ball
	{
		resetMatrices();
		modelTranslate = glm::translate(modelTranslate, ball.position);
		modelRotation = glm::rotate(modelRotation, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		modelScale = glm::scale(modelScale, ball.scale);
		renderObject(modelTranslate, modelRotation, modelScale, ball.colour, ball.texture);
		ball.render();
	}

	//Render bounds
	{
		//Left bound
		for (int i = 0; i < boundBlocks; i++)
		{
			resetMatrices();
			modelTranslate = glm::translate(modelTranslate, boundLeft[i].position);
			modelScale = glm::scale(modelScale, boundLeft[i].scale);
			modelRotation = glm::rotate(modelRotation, boundLeft[i].rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			renderObject(modelTranslate, modelRotation, modelScale, boundLeft[i].colour, boundLeft[i].texture);
			boundLeft[i].render();
		}

		//Right bound
		for (int i = 0; i < boundBlocks; i++)
		{
			resetMatrices();
			modelTranslate = glm::translate(modelTranslate, boundRight[i].position);
			modelScale = glm::scale(modelScale, boundRight[i].scale);
			modelRotation = glm::rotate(modelRotation, boundRight[i].rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			renderObject(modelTranslate, modelRotation, modelScale, boundRight[i].colour, boundRight[i].texture);
			boundRight[i].render();
		}
	
		//Top bound
		for (int i = 0; i < topBlocks; i++)
		{
			resetMatrices();
			modelTranslate = glm::translate(modelTranslate, boundTop[i].position);
			modelScale = glm::scale(modelScale, boundTop[i].scale);
			modelRotation = glm::rotate(modelRotation, boundTop[i].rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			renderObject(modelTranslate, modelRotation, modelScale, boundTop[i].colour, boundTop[i].texture);
			boundTop[i].render();
		}
	}

	//Render power up
	{
		//If power up is active
		if (powerUp.active)
		{
			resetMatrices();
			modelTranslate = glm::translate(modelTranslate, powerUp.position);
			modelScale = glm::scale(modelScale, powerUp.scale);
			renderObject(modelTranslate, modelRotation, modelScale, glm::vec3(1.0f), powerUp.texture);
			powerUp.render();
		}
	}

	//Use the shader program
	glProgram.unuse();
	glDisable(GL_DEPTH_TEST);
	glSpriteProgram.use();

	//Render lives
	{
		for (int i = 0; i < player.lives; i++)
		{
			resetMatrices();
			modelTranslate = glm::translate(modelTranslate, glm::vec3(lives.position.x + (i * 40.0f), lives.position.y, lives.position.z));
			modelScale = glm::scale(modelScale, lives.scale);
			renderSprite(modelTranslate, modelScale, lives.colour, lives.texture);
			lives.render();
		}
	}

	//Render win/gameover texture
	{
		//If winning the game render
		if (win.active)
		{
			resetMatrices();
			modelTranslate = glm::translate(modelTranslate, win.position);
			modelScale = glm::scale(modelScale, win.scale);
			renderSprite(modelTranslate, modelScale, glm::vec3(1.0f), win.texture);
			win.render();
		}

		//If lossing the game render
		if (gameover.active)
		{
			resetMatrices();
			modelTranslate = glm::translate(modelTranslate, gameover.position);
			modelScale = glm::scale(modelScale, gameover.scale);
			renderSprite(modelTranslate, modelScale, glm::vec3(1.0f), gameover.texture);
			gameover.render();
		}
	}

	//Render score
	{
		//Render the score objects
		for (auto& sprite : scoreObject)
		{
			resetMatrices();
			modelTranslate = glm::translate(modelTranslate, sprite.position);
			modelScale = glm::scale(modelScale, sprite.scale);
			renderSprite(modelTranslate, modelScale, glm::vec3(1.0f), sprite.texture);
			sprite.render();
		}
	}

	//End rendering
	endRender();
}

//Reset the model and scale matrices
void resetMatrices()
{
	modelTranslate = glm::mat4(1.0f);
	modelScale = glm::mat4(1.0f);
	modelRotation = glm::mat4(1.0f);
}

//Render objects
void renderObject(glm::mat4 translation, glm::mat4 rotation, glm::mat4 scale, glm::vec3 colour, Texture &texture)
{
	//Lighting
	glUniform3f(glGetUniformLocation(glProgram.getProgramID(), "uObjectColour"), colour.x, colour.y, colour.z);
	glUniform3f(glGetUniformLocation(glProgram.getProgramID(), "uLightColour"), lightColour.x, lightColour.y, lightColour.z);
	glUniform3f(glGetUniformLocation(glProgram.getProgramID(), "uLightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(glGetUniformLocation(glProgram.getProgramID(), "uViewPosition"), cameraPosition.x, cameraPosition.y, cameraPosition.z);

	//Model/View/Projection/Colour
	glUniformMatrix4fv(glGetUniformLocation(glProgram.getProgramID(), "uModel"), 1, GL_FALSE, glm::value_ptr(translation * rotation * scale));
	glUniformMatrix4fv(glGetUniformLocation(glProgram.getProgramID(), "uView"), 1, GL_FALSE, glm::value_ptr(glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp)));
	glUniformMatrix4fv(glGetUniformLocation(glProgram.getProgramID(), "uProjection"), 1, GL_FALSE, glm::value_ptr(glm::perspective(glm::radians(FOV), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f)));
	glBindTexture(GL_TEXTURE_2D, texture.texture);
}

//Render sprite
void renderSprite(glm::mat4 translation, glm::mat4 scale, glm::vec3 colour, Texture &texture)
{
	//Render sprites
	glUniformMatrix4fv(glGetUniformLocation(glSpriteProgram.getProgramID(), "uModel"), 1, GL_FALSE, glm::value_ptr(translation * scale));
	glUniformMatrix4fv(glGetUniformLocation(glSpriteProgram.getProgramID(), "uView"), 1, GL_FALSE, glm::value_ptr(orthoViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(glSpriteProgram.getProgramID(), "uProjection"), 1, GL_FALSE, glm::value_ptr(orthoProgMatrix));
	glUniform3f(glGetUniformLocation(glSpriteProgram.getProgramID(), "uColour"), colour.x, colour.y, colour.z);
	glBindTexture(GL_TEXTURE_2D, texture.texture);
}

//End the rendering process
void endRender()
{
	//Swap the buffer to render to screen
	window.swapBuffer();
	glSpriteProgram.unuse();
}



//Colision detection ball to brick
bool collisionDetection(Ball &ball, Brick &brick)
{
	//The sides of ball and brick
	GLfloat ballLeft, brickLeft, ballRight, brickRight, ballTop, brickTop, ballBottom, brickBottom;

	//Calculate the sides of ball
	ballLeft = ball.position.x - ball.scale.x;
	ballRight = ball.position.x + ball.scale.x;
	ballTop = ball.position.y - ball.scale.x;
	ballBottom = ball.position.y + ball.scale.x;

	//Calculate the sides of brick
	brickLeft = brick.position.x - brick.scale.x;
	brickRight = brick.position.x + brick.scale.x;
	brickTop = brick.position.y - brick.scale.x;
	brickBottom = brick.position.y + brick.scale.x;

	//If any of the sides from the ball are outside of the brick
	if (ballBottom <= brickTop) return false;
	if (ballTop >= brickBottom) return false;
	if (ballRight <= brickLeft) return false;
	if (ballLeft >= brickRight) return false;

	//If any of the sides from the ball are inside of the brick
	return true;
}

//Colision detection power up to player
bool collisionDetection(GameObject &powerUp, Player &player)
{
	//The sides of ball and player
	GLfloat powerUpLeft, playerLeft, powerUpRight, playerRight, powerUpTop, playerTop, powerUpBottom, playerBottom;

	//Calculate the sides of ball
	powerUpLeft = powerUp.position.x - powerUp.scale.x;
	powerUpRight = powerUp.position.x + powerUp.scale.x; 
	powerUpTop = powerUp.position.y - powerUp.scale.x;
	powerUpBottom = powerUp.position.y + powerUp.scale.y;

	//Calculate the sides of player
	playerLeft = player.position.x - player.scale.x;
	playerRight = player.position.x + player.scale.x;
	playerTop = player.position.y - player.scale.x;
	playerBottom = player.position.y + player.scale.y;

	//If any of the sides from the powerUp are outside of the player
	if (powerUpBottom <= playerTop) return false;
	if (powerUpTop >= playerBottom) return false;
	if (powerUpRight <= playerLeft) return false;
	if (powerUpLeft >= playerRight) return false;

	//If any of the sides from the powerUp are inside of the brick
	return true;
}

//Colision detection ball to player
bool collisionDetection(Ball &ball, Player &player)
{
	//The sides of ball and player
	GLfloat ballLeft, playerLeft, ballRight, playerRight, ballTop, playerTop, ballBottom, playerBottom;

	//Calculate the sides of ball
	ballLeft = ball.position.x - ball.scale.x;
	ballRight = ball.position.x + ball.scale.x;
	ballTop = ball.position.y - ball.scale.y;
	ballBottom = ball.position.y + ball.scale.y;

	//Calculate the sides of player
	playerLeft = player.position.x - player.scale.x;
	playerRight = player.position.x + player.scale.x;
	playerTop = player.position.y - player.scale.y;
	playerBottom = player.position.y + player.scale.y;

	//If any of the sides from the ball are outside of the player
	if (ballBottom <= playerTop) return false;
	if (ballTop >= playerBottom) return false;
	if (ballRight <= playerLeft) return false;
	if (ballLeft >= playerRight) return false;

	//If any of the sides from the ball are inside of the player
	return true;
}



//Maintain the aspect ratio
void maintainAspectRatio()
{
	//Get the window height and width
	int width;
	int height;
	int oldwidth = screenWidth;
	int oldheight = screenHeight;
	
	//Get the window size
	SDL_GetWindowSize(window.window(), &width, &height);

	//Check aspect ratio
	if (width != oldwidth) height = (GLint)(width * aspectRatio);
	else width = (GLint)(height * (1 / aspectRatio));

	//Set viewport
	glViewport(0, 0, width, height);
	SDL_SetWindowSize(window.window(), width, height);
	SDL_SetWindowPosition(window.window(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

//Toggle fullscreen
bool toggleFullScreen()
{
	//If not fullscreen - enter fullscreen
	if (!fullscreen)
	{
		SDL_SetWindowFullscreen(window.window(), SDL_WINDOW_FULLSCREEN);
		return true;
	}

	//If fullscreen - exit fullscreen
	else
	{
		SDL_SetWindowFullscreen(window.window(), 0);
		return false;
	}
}

//Load the score gameobjects
void loadScore()
{
	//Scale of numbers
	glm::vec3 scale = glm::vec3(30.0f, 30.0f, 1.0f);

	//Load in score game objects
	for (int i = 0; i < 4; i++)
	{
		//Score object
		Sprite sprite;
		sprite.setBuffers();

		//Set the score texture
		if (i == 0)
		{
			sprite.scale = glm::vec3(160.0f, 30.0f, 1.0f);
			sprite.position = glm::vec3
			(
				(GLfloat)screenWidth - 280.0f,
				(GLfloat)screenHeight - sprite.scale.y - 5.0f,
				0.0f
			);
			sprite.texture.load("..//content//newScore.png");
		}
		//Set the defualt score value textures
		else
		{
			sprite.scale = scale;
			sprite.position = glm::vec3
			(
				(GLfloat)screenWidth - 140.0f + (i * 30.0f),
				(GLfloat)screenHeight - sprite.scale.y - 5.0f,
				0.0f
			);
			sprite.texture.load("..//content//0.png");
		}
		sprite.texture.setBuffers();
		scoreObject.push_back(sprite);
	}

	//Load each number texture
	for (int i = 0; i < 10; i++)
	{
		//Texture
		Texture texture;
		std::string file = "..//content//" + std::to_string(i) + ".png";
		texture.load(file);
		texture.setBuffers();
		scoreText.push_back(texture);
	}
}

//Set the score
void setScore()
{
	//Get the score as a string
	std::string scoreStr = std::to_string(score);

	//Get the end of the vector
	int pos = static_cast<int>(scoreObject.size()) - 1;

	//Loop through the score string from the end to start
	for (int i = (int)scoreStr.length() - 1; i >= 0; i--)
	{
		int value = scoreStr[i] - '0';
		scoreObject[pos].texture = scoreText[value];
		pos--;
	}
}

//Is the game won
bool wonGame()
{
	//Check if the bricks are all dead
	for (int y = 0; y < numbBricksHigh; y++)
	{
		for (int x = 0; x < numbBricksWide; x++)
		{
			//If the bricks alive game not won
			if (bricks[y][x].brickAlive) return !bricks[y][x].brickAlive;
		}
	}
	//Game won
	return true;
}




//Set the power up of the player
void setPowerUp()
{
	//Big ball
	if (powerUps == 1)
	{
		ball.scale *= 2.0f;
		player.colour = glm::vec3(0.0f, 1.0f, 1.0f);
	}

	//Max brick hits
	else if (powerUps == 2)
	{
		maxHits();
		player.colour = glm::vec3(0.0f, 1.0f, 1.0f);
	}

	//Slow ball
	else if (powerUps == 3)
	{
		player.scale *= 1.5f;
		playerVelocity *= 1.5f;
		offset = player.scale.x;
		player.colour = glm::vec3(1.0f, 0.0f, 1.0f);
	}
}

//Max hit power up
void maxHits()
{
	//Loop through all bricks
	for (int y = 0; y < numbBricksHigh; y++)
	{
		for (int x = 0; x < numbBricksWide; x++)
		{
			//If the brick is active
			if (bricks[y][x].brickAlive) setCrackedBrick(x, y);
			if (bricks[y][x].hits < 0) setDeadBrick(x, y);
		}
	}
}

//Activate a power up
void powerUpActive(int x, int y)
{
	//If the power up is active
	if (bricks[y][x].brickDying && !powerUp.active)
	{
		powerUp.active = true;
		powerUp.position = bricks[y][x].position;
	}
}



//Crack the brick
void setCrackedBrick(int x, int y)
{
	bricks[y][x].hits -= 1;
	bricks[y][x].texture = bricks[y][x].cracked;
	score += 1;
	setScore();
}

//Disable the brick
void setDeadBrick(int x, int y)
{
	bricks[y][x].brickDying = true;
	bricks[y][x].brickAlive = false;
	score += 3;
	setScore();
}

//Set brick as dying
void setDyingBrick(int x, int y)
{
	bricks[y][x].position.y -= 9.5f * deltaTime;
	bricks[y][x].rotation += 0.075f;
	if (bricks[y][x].scale.x > 0.0f) bricks[y][x].scale -= 0.75f * deltaTime;
}



//Calculate the camera vectors
void updateCamera()
{
	//Temp vec3 to calculate the new camera front
	glm::vec3 front;

	//Calculate the x/y/z components
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	//Set the camera front
	cameraFront = glm::normalize(front);

	//Set the camera right
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));

	//Set the camera up
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

//Handle mouse input
void mouseInput(float xOffset, float yOffset, bool constrainPitch)
{
	//Calculate the x/y offset from the mouse position
	xOffset *= mouseSensitivity * deltaTime;
	yOffset *= mouseSensitivity * deltaTime;

	//Set the pitch and yaw
	yaw += xOffset;
	pitch += yOffset;

	//Constrain the pitch value between -90 and 90 to stop screen flip
	if (constrainPitch)
	{
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}
}

//Update the camera view
void updateCameraView(int view)
{
	//Switch on the camera view
	switch (view)
	{
	case 1: followPlayer = false; freeCamera = false; cameraPosition = camView1; pitch =  45.0f; yaw =  -90.0f;  break;
	case 2: followPlayer = false; freeCamera = false; cameraPosition = camView2; pitch = -30.0f; yaw =  -90.0f;  break;
	case 3: followPlayer = false; freeCamera = false; cameraPosition = camView3; pitch =   0.0f; yaw = -135.0f; break;
	case 4:	followPlayer = true;  freeCamera = false; 							 pitch =  30.0f; yaw =  -90.0f;  break;
	case 5: followPlayer = false; freeCamera = true;  cameraPosition = camView5; pitch =   0.0f; yaw =  -90.0f;  break;
	}

	//Updated the view
	updateView = false;
}