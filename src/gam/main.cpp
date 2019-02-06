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
	float deltatime = 0.18f;
	float totaltime = 0.0f;
	
	SDL_Event e;
	g_world_start();

	CTauCamera* camera = new CTauCamera(0.0, 6.0, 0.0);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetMouseState(&mx_old, &my_old);
	do
	{
		totaltime += deltatime / 10.0f;

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
			camera->Forward(1.0f * deltatime);
		if (keys[SDL_SCANCODE_DOWN])
			camera->Forward(-1.0f * deltatime);
		if (keys[SDL_SCANCODE_LEFT])
			camera->Turn(-0.05f);
		if (keys[SDL_SCANCODE_RIGHT])
			camera->Turn(0.05f);
		camera->Turn((float)(mx - mx_old) * mousesensitivity);
		camera->Recalculate();

		// Tick
		rad += 0.01f;

		g_world_tick(camera);
		tau_gra_updatewindow();
		SDL_Delay(10);

		mx_old = mx;
		my_old = my;
	} while (r);

	delete(camera);

	g_world_quit();
}
