#include <glm/glm.hpp>
#include "../eng/render.h"
#include "../eng/camera.h"
#include "../eng/print.h"
#include "world_load.h"
#include "world.h"

// Persistent data
#define shader_default  shaders[0]
#define shader_text     shaders[1]
#define shader_screen   shaders[2]
#define shader_backdrop shaders[3]
static SHADER      shaders[4];
static TEXTURE     textures[2];
static FONT        font_default;
static FRAMEBUFFER framebuffer;

// Current world data
static MESH mesh_walls;

void g_world_start(void)
{
	std::vector<glm::vec4> collisions;
	g_world_load("world0", collisions);
	
	// Load persistent data
	mesh_walls       = tau_gra_mesh_make("data/models/world0.obj");
	shader_default   = tau_gra_shader_make("data/shaders/default_vtx.glsl", "data/shaders/default_frg.glsl");
	shader_text      = tau_gra_shader_make("data/shaders/font_vtx.glsl", "data/shaders/font_frg.glsl");
	shader_screen    = tau_gra_shader_make("data/shaders/screen_vtx.glsl", "data/shaders/screen_frg.glsl");
	shader_backdrop  = tau_gra_shader_make("data/shaders/backdrop_vtx.glsl", "data/shaders/backdrop_frg.glsl");
	font_default     = tau_gra_font_make("data/fonts/default.ttf", 32);
	textures[0]      = tau_gra_texture_make("data/textures/checkerboard.png");
	textures[1]      = tau_gra_texture_make("data/textures/wall0.png");
	framebuffer      = tau_gra_framebuffer_make(800, 600);
}

void g_world_tick(CTauCamera* camera, float delta, int fps)
{
	glm::mat4 text_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	glm::mat4 identity        = glm::mat4(1.0f);
	
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

	// Walls
	tau_gra_shader_setuniformMat4(&shader_default, "model", glm::value_ptr(identity));
	tau_gra_shader_setuniformFlt1(&shader_default, "uvscale", 1.0f);
	tau_gra_texture_use(&textures[1], TAU_TEXTUREUNIT_0);
	tau_gra_ren_mesh(&mesh_walls);

	// Text
	tau_gra_shader_use(&shader_text);
	tau_gra_shader_setuniformInt1(&shader_text, "texture0", 0);
	tau_gra_shader_setuniformMat4(&shader_text, "proj", glm::value_ptr(text_projection));
	tau_gra_disableDepthTest();
	tau_gra_font_rendertext(&font_default, std::string("$ cat nyx > talk :: frames/second: ") + std::to_string(fps), 0, 16, 1.0f);
	tau_gra_enableDepthTest();

	// Render to screen
	tau_gra_framebuffer_use(nullptr);
	tau_gra_clear(TAU_CLEAR_DEPTHBUFFER);
	tau_gra_shader_use(&shader_screen);
	tau_gra_shader_setuniformInt1(&shader_screen, "texture0", 0);
	tau_gra_texture_use(&framebuffer.attachedtexture, TAU_TEXTUREUNIT_0);
	tau_gra_ren_mesh_unitsquare();
}

void g_world_quit(void)
{
	tau_gra_framebuffer_destroy(&framebuffer);
	tau_gra_font_destroy(&font_default);
	tau_gra_shader_destroy(&shader_backdrop);
	tau_gra_shader_destroy(&shader_default);
	tau_gra_shader_destroy(&shader_text);
	tau_gra_shader_destroy(&shader_screen);
	tau_gra_texture_destroy(&textures[1]);
	tau_gra_texture_destroy(&textures[0]);
	tau_gra_mesh_delete(&mesh_walls);
}