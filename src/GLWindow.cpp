#include "GLWindow.h"


//Constructor for the GLWindow
GLWindow::GLWindow() {}


//Destructor for the GLWindow
GLWindow::~GLWindow() { clean(); }


//Initialisation function to initialise the game window, initialise OpenGL context and GLEW
void GLWindow::init(std::string name, int &w, int &h)
{
	//SDL initialise
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)	SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n", SDL_GetError());

	//Get the display mode
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	auto width = DM.w;
	auto height = DM.h;
	w = width / 2;
	h = height / 2;

	//Major and minor OpenGL version
	int major = 4;
	int minor = 3;

	//Set OpenGL major and minor verison
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Print out
	SDL_Log("OpenGL Version %i.%i", major, minor);

	//Window Creation - modified for OpenGL
	//If the window is not assigned show error and end applictaion
	win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (win == nullptr)	SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow init error");
	else SDL_Log("SDL initialised OK!\n");

	//Create an OpenGL context associated with the window
	//If the gl context is not assigned show error and end applictaion
	context = SDL_GL_CreateContext(win);
	if (context == nullptr)	SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext init error");
	else SDL_Log("SDL GL context created!");

	//Sets the viewport as that of the game screen dimensions
	glViewport(0, 0, w, h);

	//Check the KHR_debug extension is supported
	if (GL_KHR_debug) SDL_Log("KHR_debug supported.\n");

	//Check for debug context
	GLint context;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context);
	if (GL_CONTEXT_FLAG_DEBUG_BIT) SDL_Log("OGL debug context loaded.\n");

	//Initialize GLEW
	//If GLEW failed to initailise show error and end applictaion
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Glew init error");
	SDL_Log("OpenGL version is %s", glGetString(GL_VERSION));

	//Enable depth test
	glEnable(GL_DEPTH_TEST);

	//Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//Enable the OpenGL alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


//SwapBuffer - function to update the game window with OpenGL rendering
void GLWindow::swapBuffer() { SDL_GL_SwapWindow(win); }


//Get the game window
SDL_Window* GLWindow::window() { return win; }


//Accessor for screen width
int GLWindow::screenWidth() { return width; }


//Accessor for screen height
int GLWindow::screenHeight() { return height; }


//Clean function to clean up the game window and OpenGl context
void GLWindow::clean()
{
	//Once finished with OpenGL functions, the SDL_GLContext can be deleted
	SDL_DestroyWindow(win);
	SDL_GL_DeleteContext(context);
	SDL_Quit();
}