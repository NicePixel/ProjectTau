#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../eng/render.h"
#include "../eng/camera.h"
#include "../eng/print.h"
#include "../eng/a_main.h"
#include "world.h"
#include "entity.h"

// Persistent data
static SHADER      shaders[5];
static TEXTURE     textures[3];
static FONT        font_default;
static FRAMEBUFFER framebuffer;

// Current world data
static float totaltime;

// This is a test
static MESH part0, part1;
static MESH panel;
static TEXTURE  janimation[30];
static TEXTURE* janimation_texture;

SHADER g_world_getshader(unsigned int index)
{
	return shaders[index];
}

TEXTURE g_world_gettexture(unsigned int index)
{
	return textures[index];
}

FONT g_world_getfont(void)
{
	return font_default;
}

#undef  TED_CURSUB
#define TED_CURSUB "g_world_start_persistent"
void g_world_start_persistent(void)
{
	// Load persistent data
	part0            = tau_gra_mesh_make("data/models/part0.obj");
	part1            = tau_gra_mesh_make("data/models/part1.obj");
	panel            = tau_gra_mesh_make("data/models/panel.obj");
	shader_default   = tau_gra_shader_make("data/shaders/default_vtx.glsl", "data/shaders/default_frg.glsl");
	shader_text      = tau_gra_shader_make("data/shaders/font_vtx.glsl", "data/shaders/font_frg.glsl");
	shader_screen    = tau_gra_shader_make("data/shaders/screen_vtx.glsl", "data/shaders/screen_frg.glsl");
	shader_backdrop  = tau_gra_shader_make("data/shaders/backdrop_vtx.glsl", "data/shaders/backdrop_frg.glsl");
	shader_loading   = tau_gra_shader_make("data/shaders/loading_vtx.glsl", "data/shaders/loading_frg.glsl");
	font_default     = tau_gra_font_make("data/fonts/default.ttf", 32);
	textures[0]      = tau_gra_texture_make("data/textures/checkerboard.png");
	textures[2]      = tau_gra_texture_make("data/textures/warn.png");
	framebuffer      = tau_gra_framebuffer_make(800, 600);
	for (unsigned int i =0 ; i < 30; i++)
	{
		std::string fn = "data/textures/f";
		fn += std::to_string(i);
		fn += ".png";
		janimation[i] = tau_gra_texture_make(fn.c_str());
	}
	janimation_texture = &janimation[0];
}

#undef  TED_CURSUB
#define TED_CURSUB "g_world_start"
void g_world_start(CTauCamera** newcamera)
{
	totaltime  = 0.0f;
	*newcamera = new CTauCamera(0.0f, 0.0f, 0.0f);
	
	/* -------------------------
	 * This is really not needed.
	 * The world consists of only three models, carousel, which is two models,
	 * and the jevil character.
	 * World loading is unnecessary here.
	 * -------------------------
	 */
	/*
	g_world_load("world0", collisions, entity, &entity_len);
	
	// The created camera should be where the player spawns.
	// Find the player spawner.
	for (int i = 0; i < entity_len; i++)
	{
		ENTITY e = entity[i];
		switch(e.eid)
		{
			case EID_PLAYERSPAWN:
				*newcamera = new CTauCamera((float) e.x, CAMERA_HEIGHT, (float)e.y);
				(*newcamera)->Turn((float)(e.angle) * (-3.1415f / 180.0f));
				(*newcamera)->Recalculate();
			default:
				break;
		}
	}
	if (!newcamera)
	{
		TED_PRINT_ERROR("There is no camera entity, \"EID=1\"!!!");
	}
	*/
}

#undef  TED_CURSUB
#define TED_CURSUB "g_world_tick"
void g_world_tick(CTauCamera* camera, float delta, int fps, const Uint8* keys, int mousedeltax, uint32_t frame)
{
	glm::mat4 text_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	glm::mat4 identity        = glm::mat4(1.0f);
	totaltime += delta;
	
	// Draw (to the framebuffer)
	tau_gra_framebuffer_use(&framebuffer);
	tau_gra_clear(TAU_CLEAR_DEPTHBUFFER);
	
	// Backdrop, the ground and the ceiling
	// The colouring is handled withing the shader itself.
	tau_gra_shader_use(&shader_backdrop);
	tau_gra_disableDepthTest();
	tau_gra_ren_mesh_unitsquare();
	tau_gra_enableDepthTest();
	
	// Prepare the default shader
	tau_gra_shader_use(&shader_default);
	tau_gra_shader_setuniformInt1(&shader_default, "texture0", 0);
	tau_gra_shader_setuniformInt1(&shader_default, "onlycolor", 0);
	tau_gra_shader_setuniformMat4(&shader_default, "view", camera->GetValueView());
	tau_gra_shader_setuniformMat4(&shader_default, "proj", camera->GetValueProjection());

	// World
	glm::mat4 model  = glm::scale(identity, glm::vec3(4.0,  4.0, 4.0));
	glm::mat4 modelp = identity;
	modelp           = glm::translate(modelp, glm::vec3(32.0,  8.0, 0.0));
	modelp           = glm::rotate   (modelp, 3.1415f, glm::vec3(0.0,  1.0, 0.0));
	modelp           = glm::scale    (modelp, glm::vec3(8.0,  8.0, 8.0));
	glm::vec3 color0 = glm::vec3( 2.0f/100.0f,  2.4f/100.0f, 47.5f/100.0f);
	glm::vec3 color1 = glm::vec3(12.2f/100.0f, 12.5f/100.0f, 37.3f/100.0f);
	
	// Carousel
	tau_gra_shader_setuniformMat4(&shader_default, "model", glm::value_ptr(model));
	tau_gra_shader_setuniformInt1(&shader_default, "onlycolor", 1);
	tau_gra_shader_setuniformFlt3(&shader_default, "tintcolor", glm::value_ptr(color0));
	tau_gra_ren_mesh(&part0);
	tau_gra_shader_setuniformFlt3(&shader_default, "tintcolor", glm::value_ptr(color1));
	tau_gra_ren_mesh(&part1);
	
	// Jevil
	tau_gra_shader_setuniformInt1(&shader_default, "onlycolor", 0);
	tau_gra_shader_setuniformMat4(&shader_default, "model", glm::value_ptr(modelp));
	tau_gra_shader_setuniformFlt1(&shader_default, "uvscale", 1.0f);
	tau_gra_texture_use(janimation_texture, TAU_TEXTUREUNIT_0);
	tau_gra_ren_mesh(&panel);

	// Text
	tau_gra_shader_use(&shader_text);
	tau_gra_shader_setuniformInt1(&shader_text, "texture0", 0);
	tau_gra_shader_setuniformFlt1(&shader_text, "totaltime", totaltime);
	tau_gra_shader_setuniformMat4(&shader_text, "proj", glm::value_ptr(text_projection));
	tau_gra_disableDepthTest();
	tau_gra_font_rendertext(&font_default, std::string("[") + std::to_string(fps) + std::string("]"), 0, 2, 1.0f);
	tau_gra_enableDepthTest();

	// Render to screen
	tau_gra_framebuffer_use(nullptr);
	tau_gra_clear(TAU_CLEAR_DEPTHBUFFER);
	tau_gra_shader_use(&shader_screen);
	tau_gra_shader_setuniformInt1(&shader_screen, "texture0", 0);
	tau_gra_texture_use(&framebuffer.attachedtexture, TAU_TEXTUREUNIT_0);
	tau_gra_ren_mesh_unitsquare();
	
	// Update the world
	const float to_rad = 3.1415f / 180.0f;
	int   angle        = ((frame/15) % 360);
	int   distance     = 800;
	int   vol          = ((-sin((angle-90) * to_rad)+1)/2.0 * 64) + 64;
	float rad          = -angle * to_rad;
	camera->SetPosition(8*cos(rad), 8.0f, 8*sin(rad));
	camera->ForceLook(10*cos(rad), 8.0f, 10*sin(rad));
	tau_aud_test(angle, distance);
	tau_aud_vol(vol);
	janimation_texture = &janimation[(angle/4)%30];
}

void g_world_quit(void)
{
	tau_gra_framebuffer_destroy(&framebuffer);
	tau_gra_font_destroy(&font_default);
	tau_gra_shader_destroy(&shader_backdrop);
	tau_gra_shader_destroy(&shader_default);
	tau_gra_shader_destroy(&shader_text);
	tau_gra_shader_destroy(&shader_screen);
	tau_gra_texture_destroy(&textures[2]);
	tau_gra_texture_destroy(&textures[0]);
	tau_gra_mesh_delete(&panel);
	tau_gra_mesh_delete(&part0);
	tau_gra_mesh_delete(&part1);
	for (unsigned int i =0 ; i < 30; i++)
	{
		tau_gra_texture_destroy(&janimation[i]);
	}
}