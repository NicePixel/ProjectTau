#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../eng/render.h"
#include "../eng/camera.h"
#include "../eng/print.h"
#include "../eng/a_main.h"
#include "world_load.h"
#include "world.h"
#include "entity.h"

// Persistent data
static SHADER      shaders[5];
static TEXTURE     textures[3];
static FONT        font_default;
static FRAMEBUFFER framebuffer;

// Current world data
#define CAMERA_HEIGHT 8.0f
static MESH   mesh_walls;
static ENTITY entity[16];
static int    entity_len;
static std::vector<glm::vec4> collisions;
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
	mesh_walls       = tau_gra_mesh_make("data/models/world0.obj");
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
	textures[1]      = tau_gra_texture_make("data/textures/becareful.png");
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
#include "entity_table.h"
void g_world_start(CTauCamera** newcamera)
{
	totaltime = 0.0f;
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
}

static bool intersect(glm::vec2 p1, glm::vec2 p2, glm::vec2 q1, glm::vec2 q2)
{
    return (((q1.x-p1.x)*(p2.y-p1.y) - (q1.y-p1.y)*(p2.x-p1.x)) * ((q2.x-p1.x)*(p2.y-p1.y) - (q2.y-p1.y)*(p2.x-p1.x)) < 0) &&
           (((p1.x-q1.x)*(q2.y-q1.y) - (p1.y-q1.y)*(q2.x-q1.x)) * ((p2.x-q1.x)*(q2.y-q1.y) - (p2.y-q1.y)*(q2.x-q1.x)) < 0);
}

// Handle player movement
// Recalculate camera's matrices only if its state changes (moving, rotating...)
#define RADIUS_COLLISION 128.0f
#define sgn(x) (x >= 0.0f ? 1.0f : -1.0)
void movement(CTauCamera** camera, float delta, const Uint8* keys, int mousedeltax)
{
	const float movementSpeed = 16.0f   * delta;
	const float lookSpeed     = 3.1415f * delta;
	bool  recalculate         = false;
	glm::vec2 vecmove         = glm::vec2(0.0f, 0.0f);
	
	// Handle key presses
	if (keys[SDL_SCANCODE_UP])
	{
		vecmove     = (*camera)->GetForwardVector(+movementSpeed);
		recalculate = true;
	}
	else if (keys[SDL_SCANCODE_DOWN])
	{
		vecmove     = (*camera)->GetForwardVector(-movementSpeed);
		recalculate = true;
	}
	if (keys[SDL_SCANCODE_LEFT])
	{
		(*camera)->Turn(-lookSpeed);
		recalculate = true;
	}
	else if (keys[SDL_SCANCODE_RIGHT])
	{
		(*camera)->Turn(+lookSpeed);
		recalculate = true;
	}
	
	// Handle mouse
	if ((float)(mousedeltax) > 0.0f || (float)(mousedeltax) < 0.0f)
	{
		(*camera)->Turn((float)(mousedeltax) * 0.01f);
		recalculate = true;
	}
	
	// For every wall's line,
	// * calculate the line's relative angle to the coordinate system.
	//   `wallangle`
	// * find the length (magnitude) of the player's current position and the
	// new, wanted position (forward vector). `m0` `m1`
	// * If the line the player's move vector and wall's vector intersect,
	// find the angle of the intersection. (the inner angle) `angle`
	// * Intersection will make the player move alongside the wall, the side of 
	// moving depends on the inner angle of the intersection.
	glm::vec3 camerapos = (*camera)->GetPosition();
	glm::vec2 cam_p0    = glm::vec2(camerapos.x, camerapos.z);
	glm::vec2 cam_p1    = glm::vec2(camerapos.x + vecmove.x * RADIUS_COLLISION, camerapos.z + vecmove.y * RADIUS_COLLISION);
	for (unsigned int i = 0; i < collisions.size(); i++)
	{
		glm::vec4 wall    = collisions.at(i);
		glm::vec2 wall_p0 = glm::vec2(wall.x, wall.y);
		glm::vec2 wall_p1 = glm::vec2(wall.z, wall.w);
		float wallangle   = atan2(wall_p1.y - wall_p0.y, wall_p1.x - wall_p0.x);
		if (intersect(cam_p0, cam_p1, wall_p0, wall_p1))
		{
			float dx0   = cam_p0.x  - cam_p1.x;
			float dy0   = cam_p0.y  - cam_p1.y;
			float dx1   = wall_p0.x - wall_p1.x;
			float dy1   = wall_p0.y - wall_p1.y;
			float m0    = sqrt(dx0*dx0 + dy0*dy0);
			float m1    = sqrt(dx1*dx1 + dy1*dy1);
			float angle = acos((dx0*dx1 + dy0*dy1) / (m0 * m1));
			vecmove.x   = sgn(angle) * vecmove.x * abs(cos(wallangle));
			vecmove.y   = sgn(angle) * vecmove.y * abs(sin(wallangle));
		}
	}
	
	if (recalculate)
	{
		(*camera)->Move(vecmove.x, 0.0f, vecmove.y);
		(*camera)->Recalculate();
	}
}
#undef sgn
#include <SDL2/SDL_mixer.h>
#undef  TED_CURSUB
#define TED_CURSUB "g_world_tick"
void g_world_tick(CTauCamera* camera, float delta, int fps, const Uint8* keys, int mousedeltax, uint32_t frame)
{
	glm::mat4 text_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	glm::mat4 identity        = glm::mat4(1.0f);
	totaltime += delta;
	
	//movement(&camera, delta, keys, mousedeltax);
	
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
	//tau_gra_ren_mesh(&mesh_walls);
	// World test
	glm::mat4 model  = glm::scale(identity, glm::vec3(4.0,  4.0, 4.0));
	glm::mat4 modelf = glm::scale(identity, glm::vec3(4.0, -4.0, 4.0));
	glm::mat4 modelp = identity;
	modelp = glm::translate(modelp, glm::vec3(32.0,  8.0, 0.0));
	modelp = glm::rotate(modelp, 3.1415f/1.0f, glm::vec3(0.0,  1.0, 0.0));
	modelp = glm::scale(modelp, glm::vec3(8.0,  8.0, 8.0));
	glm::vec3 color0 = glm::vec3(2.0f/100.0f, 2.4f/100.0f, 47.5f/100.0f);
	glm::vec3 color1 = glm::vec3(12.2f/100.0f, 12.5f/100.0f, 37.3f/100.0f);
	tau_gra_shader_setuniformMat4(&shader_default, "model", glm::value_ptr(model));
	tau_gra_shader_setuniformInt1(&shader_default, "onlycolor", 1);
	tau_gra_shader_setuniformFlt3(&shader_default, "tintcolor", glm::value_ptr(color0));
	tau_gra_ren_mesh(&part0);
	tau_gra_shader_setuniformFlt3(&shader_default, "tintcolor", glm::value_ptr(color1));
	tau_gra_ren_mesh(&part1);
	tau_gra_shader_setuniformMat4(&shader_default, "model", glm::value_ptr(modelf));
	tau_gra_shader_setuniformFlt3(&shader_default, "tintcolor", glm::value_ptr(color0));
	tau_gra_ren_mesh(&part0);
	tau_gra_shader_setuniformFlt3(&shader_default, "tintcolor", glm::value_ptr(color1));
	tau_gra_ren_mesh(&part1);
	
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
	
	// Audio test ??
	// ?????
	int angle = ((frame/15) % 360);
	int distance = 800;
	int vol = ((-sin((angle-90)*3.1415f/180.0f)+1)/2.0 * 64) + 64;
	Mix_Volume(2, vol);
	float rad = -angle * 3.1415f/180.0f;
	camera->SetPosition(8*cos(rad), 8.0f, 8*sin(rad));
	camera->ForceLook(10*cos(rad), 8.0f, 10*sin(rad));
	tau_aud_test(angle, distance);
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
	tau_gra_texture_destroy(&textures[1]);
	tau_gra_texture_destroy(&textures[0]);
	tau_gra_mesh_delete(&mesh_walls);
}