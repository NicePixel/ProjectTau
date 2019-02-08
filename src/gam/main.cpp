#include "SDL2/SDL.h"
#include "../eng/entry.h"
#include "../eng/print.h"

// SDL already defines main() function which initializes.
// Since we're defining our own main(), we tell SDL that we'll take care of it.
#define SDL_MAIN_HANDLED 
#undef main

static void knot(void);

#undef  TED_CURSUB
#define TED_CURSUB "main"
int main(int argc, char* argv[])
{
	SDL_SetMainReady();
	TED_PRINT_INFO("Hello");
	tau_init("Karrot 2.34837D");
	knot();
	tau_quit();
	TED_PRINT_INFO("Goodbye");
	return 0;
}

// `fps_tick` calculates the time difference between calls to itself.
// The output is the delta time, the time taken per frame, in milliseconds.
// `fps` is the amount of frames per second.
//
// Notice that it takes at least 'FRAMETIME_SIZE' frames to get an accurate
// result. 
// The first FRAMETIME_SIZE values can and will be incorrect.
//
// At worst, every frame may take 1 millisecond to render, then the delta is 
// average over FRAMETIME_SIZE frames... delta will be (1 / FRAMETIME_SIZE)...
#undef  TED_CURSUB
#define TED_CURSUB "fps_tick"
#define FRAMETIME_SIZE 512
static float frametimes[FRAMETIME_SIZE];
void fps_tick(float* delta, int* fps)
{
	static Uint32 timenow = 0;
	static Uint32 timeold = 0;
	static Uint32 frame   = 0;
	float avgframetime    = 0.0f;
	
	timenow = SDL_GetTicks();
	frametimes[frame % FRAMETIME_SIZE] = (float)(timenow - timeold);
	frame++;
	
	for (unsigned int i = 0; i < FRAMETIME_SIZE; i++)
	{
		avgframetime += frametimes[i];
	}
	avgframetime /= FRAMETIME_SIZE;
	
	timeold = timenow;
	*delta  = avgframetime / 1000.0f;
	*fps    = (int) (1000.0f / avgframetime);
}

#include "../eng/object.h"
#include "../eng/camera.h"
#include "../eng/render.h"
#include "world.h"
#undef  TED_CURSUB
#define TED_CURSUB "knot"
void knot(void)
{
	int r = 1, mx, mx_old, my, my_old;
	float rad = 0.0f, mousesensitivity = 0.01f;
	
	SDL_Event e;
	g_world_start();

	CTauCamera* camera = new CTauCamera(0.0, 6.0, 0.0);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetMouseState(&mx_old, &my_old);
	do
	{
		// Timing
		float delta;
		int fps;
		fps_tick(&delta, &fps);

		// Events
		SDL_PumpEvents();
		const Uint8* keys = SDL_GetKeyboardState(0);
		SDL_GetMouseState(&mx, &my);
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				r = 0;
		}
		if (keys[SDL_SCANCODE_F2])
		{
			TVIDEO_INFO vid = tau_gra_videogetinfo();
			if (vid.mode == TAU_VIDEOMODE_FULLSCREEN)
			{
				tau_gra_videomode(TAU_VIDEOMODE_WINDOW);
			}
			else
			{
				tau_gra_videomode(TAU_VIDEOMODE_FULLSCREEN);
			}
		}
		if (keys[SDL_SCANCODE_UP])
			camera->Forward(16.0f * delta);
		if (keys[SDL_SCANCODE_DOWN])
			camera->Forward(-16.0f * delta);
		if (keys[SDL_SCANCODE_LEFT])
			camera->Turn(-3.1415 * delta);
		if (keys[SDL_SCANCODE_RIGHT])
			camera->Turn(3.1415 * delta);
		camera->Turn((float)(mx - mx_old) * mousesensitivity);
		camera->Recalculate();

		// Tick
		rad += 0.01f;

		g_world_tick(camera, delta, fps);
		tau_gra_updatewindow();

		mx_old  = mx;
		my_old  = my;
	} while (r);

	delete(camera);

	g_world_quit();
}
