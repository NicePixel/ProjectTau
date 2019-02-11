#include <stdint.h>
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

#include "../eng/object.h"
#include "../eng/camera.h"
#include "../eng/render.h"
#include "world.h"
#undef  TED_CURSUB
#define TED_CURSUB "knot"
void knot(void)
{
	int r = 1, mx, mx_old, my, my_old;
	
	SDL_Event e;
	
	CTauCamera* camera = nullptr;
	
	// Begin the world...
	g_world_start_persistent();
	{
		SHADER  sha     = g_world_getshader (shader_loading_index);
		SHADER  shatext = g_world_getshader (shader_text_index);
		TEXTURE tex     = g_world_gettexture(texture_exclamation);
		FONT    font    = g_world_getfont   ();
		glm::mat4 text_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
		
		// This is a concept, remove later!
		tau_gra_disableDepthTest();
			
		// Icon
		tau_gra_clear(TAU_CLEAR_COLORANDDEPTHBUFFER);
		tau_gra_shader_use(&sha);
		tau_gra_shader_setuniformInt1(&sha, "texture0", 0);
		tau_gra_texture_use(&tex, TAU_TEXTUREUNIT_0);
		tau_gra_ren_mesh_unitsquare();
		
		// loadtrak
		tau_gra_shader_use(&shatext);
		tau_gra_shader_setuniformInt1(&shatext, "texture0", 0);
		tau_gra_shader_setuniformFlt1(&shatext, "totaltime", 0.0f);
		tau_gra_shader_setuniformInt1(&shatext, "rainbow", 0);
		tau_gra_shader_setuniformMat4(&shatext, "proj", glm::value_ptr(text_projection));
		tau_gra_font_rendertext(&font, "Persistent data put into memory.",   0, 2+32, 1.75f);
		tau_gra_font_rendertext(&font, "Starting world in a few seconds...", 0, 2,    1.75f);
		tau_gra_updatewindow();
		
		g_world_start(&camera);
		if (!camera)
		{
			TED_PRINT_ERROR("Camera is null...");
		}
		tau_gra_enableDepthTest();
	}

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

		g_world_tick(camera, delta, fps, keys, mx - mx_old, frame);
		tau_gra_updatewindow();

		mx_old  = mx;
		my_old  = my;
	} while (r);

	delete(camera);

	g_world_quit();
}
