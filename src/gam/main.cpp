#include <stdint.h>
#include "SDL2/SDL.h"
#include "../eng/entry.h"
#include "../eng/print.h"
#include "../eng/camera.h"
#include "../eng/render.h"
#include "world.h"

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
#define FRAMETIME_SIZE 256
static float frametimes[FRAMETIME_SIZE];
static uint32_t frame = 0;
void fps_tick(float* delta, int* fps)
{
	static Uint32 timenow = 0;
	static Uint32 timeold = 0;
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

// After the persistent data has been loaded, a world can be start.
// This shows a message that the persistent data is loaded and the world is
// about to begin.
#undef  TED_CURSUB
#define TED_CURSUB "begin_world"
void begin_world(CTauCamera** camera)
{
	SHADER  sha     = g_world_getshader (shader_loading_index);
	SHADER  shatext = g_world_getshader (shader_text_index);
	TEXTURE tex     = g_world_gettexture(texture_exclamation);
	FONT    font    = g_world_getfont   ();

	// Disable depth testing throughout the loading screen, as it is
	// not needed. This is more of a 2D loading screen.
	// Depth testing is enabled afterwards.
	tau_gra_disableDepthTest();

	// Icon
	tau_gra_framebuffer_use(nullptr);
	tau_gra_clear(TAU_CLEAR_COLORANDDEPTHBUFFER);
	tau_gra_shader_use(&sha);
	tau_gra_shader_setuniformInt1(&sha, "texture0", 0);
	tau_gra_texture_use(&tex, TAU_TEXTUREUNIT_0);
	tau_gra_ren_mesh_unitsquare();
	
	// Show the scene, re-enable the depth testing.
	tau_gra_updatewindow();
	tau_gra_enableDepthTest();

	// Start the world,
	// We should check to see if the important entities exist in the
	// world. If they do not, we tell what's happened.
	g_world_start(camera);
	if (!(*camera))
	{
		TED_PRINT_ERROR("Camera is null...");
	}
}

#undef  TED_CURSUB
#define TED_CURSUB "knot"
void knot(void)
{
	int r = 1, mx, mx_old, my, my_old;
	SDL_Event e;
	
	// Very important entities!
	CTauCamera* camera = nullptr;
	
	// Begin the world...
	g_world_start_persistent();
	begin_world(&camera);

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
			{
				r = 0;
			}
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

		g_world_tick(camera, delta, fps, keys, mx - mx_old, frame);
		tau_gra_updatewindow();

		mx_old  = mx;
		my_old  = my;
	} while (r);

	delete(camera);

	g_world_quit();
}
