#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "SDL_thread.h"
#include <string>

#undef main
#include "Renderer.h"
#include "InputHandler.h"
#include "Netcode.h"

void Init();

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

InputHandler inputHandler = InputHandler();
Netcode netcode = Netcode();
string SERVERIP = "149.153.102.39";
SDL_Thread *thread;
int threadReturnValue = 0;

static int CheckForData(void *ptr);
bool quit = false;

int main()
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;


	//SDL
#pragma region SDL STUFF
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		
		Init();
		if (NULL == thread) {
			printf("\nSDL_CreateThread failed: %s\n", SDL_GetError());
		}
		else {
			SDL_WaitThread(thread, &threadReturnValue);
			printf("\nThread returned value: %d", threadReturnValue);
		}

		SDL_Event e;

		return 0;
	}
}

static int CheckForData(void* ptr)
{
	while (!quit)
	{
		netcode.CheckForData();
	}

	threadReturnValue = 1;
	return threadReturnValue;
}

void Init()
{
	netcode.Init(SERVERIP, 9999, 9998);
	// Simply create a thread
	thread = SDL_CreateThread(CheckForData, "CheckForData", (void *)NULL);
}
