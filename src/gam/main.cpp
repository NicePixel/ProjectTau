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
#undef  TED_CURSUB
#define TED_CURSUB "knot"
void knot(void)
{
	int r = 1, mx, mx_old, my, my_old;
	float rad = 0.0f, mousesensitivity = 0.01f;
	float deltatime = 0.18f;
	float totaltime = 0.0f;
	
	SDL_Event e;

	// Resources
	MESH mesh_terrain       = tau_gra_mesh_make("data/models/map0.obj");
	SHADER shader_default   = tau_gra_shader_make("data/shaders/default_vtx.glsl", "data/shaders/default_frg.glsl");
	SHADER shader_text      = tau_gra_shader_make("data/shaders/font_vtx.glsl", "data/shaders/font_frg.glsl");
	SHADER shader_screen    = tau_gra_shader_make("data/shaders/screen_vtx.glsl", "data/shaders/screen_frg.glsl");
	TEXTURE texture_chk     = tau_gra_texture_make("data/textures/checkerboard.png");
	FONT font_default       = tau_gra_font_make("data/fonts/default.ttf", 32);
	FRAMEBUFFER framebuffer = tau_gra_framebuffer_make(800, 600);

	// Objects or entities
	CTauCamera*     camera      = new CTauCamera(0.0, 6.0, 0.0);
	CTauObject*     obj_terrain = new CTauObject(0.0, 0.0, 0.0, -1, &mesh_terrain);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetMouseState(&mx_old, &my_old);
	do
	{
		totaltime += deltatime / 10.0f;

		//glm::vec3 lightpos = glm::vec3(4.0 * cos(totaltime), 4.0, 4.0 * sin(totaltime));
		glm::vec3 lightpos = glm::vec3(4.0, 4.0, -4.0);
		glm::mat4 text_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);


		SDL_PumpEvents();
		const Uint8* keys = SDL_GetKeyboardState(0);
		SDL_GetMouseState(&mx, &my);
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				r = 0;
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

		// Draw (to the framebuffer)
		tau_gra_framebuffer_use(&framebuffer);
		tau_gra_clear(TAU_CLEAR_COLORANDDEPTHBUFFER);
		tau_gra_shader_use(&shader_default);
		tau_gra_shader_setuniformInt1(&shader_default, "texture0", 0);
		tau_gra_shader_setuniformInt1(&shader_default, "onlycolor", 0);
		tau_gra_shader_setuniformMat4(&shader_default, "view",     camera->GetValueView());
		tau_gra_shader_setuniformMat4(&shader_default, "proj",     camera->GetValueProjection());
		tau_gra_shader_setuniformFlt3(&shader_default, "lightpos", glm::value_ptr(lightpos));

		// Terrain
		tau_gra_shader_setuniformMat4(&shader_default, "model", obj_terrain->GetValueMat4Position());
		tau_gra_shader_setuniformFlt1(&shader_default, "uvscale", 1.0f);
		tau_gra_texture_use(&texture_chk, TAU_TEXTUREUNIT_0);
		tau_gra_ren_mesh(obj_terrain->GetMesh());

		// Text
		tau_gra_shader_use(&shader_text);
		tau_gra_shader_setuniformInt1(&shader_text, "texture0", 0);
		tau_gra_shader_setuniformMat4(&shader_text, "proj", glm::value_ptr(text_projection));
		tau_gra_font_rendertext(&font_default, "$ cat nyx > talk", 0, 16, 1.0f);

		// Render to screen
		tau_gra_framebuffer_use(nullptr);
		tau_gra_clear(TAU_CLEAR_DEPTHBUFFER);
		tau_gra_shader_use(&shader_screen);
		tau_gra_shader_setuniformInt1(&shader_screen, "texture0", 0);
		tau_gra_texture_use(&framebuffer.attachedtexture, TAU_TEXTUREUNIT_0);
		tau_gra_ren_mesh_unitsquare();

		tau_gra_updatewindow();
		SDL_Delay(10);

		mx_old = mx;
		my_old = my;
	} while (r);

	delete(camera);
	delete(obj_terrain);

	tau_gra_framebuffer_destroy(&framebuffer);
	tau_gra_font_destroy(&font_default);
	tau_gra_shader_destroy(&shader_default);
	tau_gra_shader_destroy(&shader_text);
	tau_gra_shader_destroy(&shader_screen);
	tau_gra_texture_destroy(&texture_chk);
	tau_gra_mesh_delete(&mesh_terrain);

}
