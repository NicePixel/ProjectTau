#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../eng/render.h"
#include "../eng/camera.h"
#include "../eng/print.h"
#include "world_load.h"
#include "world.h"
#include "entity.h"

// Persistent data
static MESH        mesh_panel, mesh_crate, mesh_indent;
static SHADER      shaders[5];
static TEXTURE     textures[4];
static FONT        font_default;
static FRAMEBUFFER framebuffer;
static TVIDEO_INFO video;

// Current world data
#define CAMERA_HEIGHT 8.0f
static WORLD  thisworld;
static std::vector<glm::vec4> collisions;
static float totaltime;
static ENTITY* hands;

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
	video = tau_gra_videogetinfo();

	// Load persistent data
	mesh_panel       = tau_gra_mesh_make("data/models/panel.obj");
	mesh_crate       = tau_gra_mesh_make("data/models/crate.obj");
	mesh_indent      = tau_gra_mesh_make("data/models/indent.obj");
	shader_default   = tau_gra_shader_make("data/shaders/default.json");
	shader_screen    = tau_gra_shader_make("data/shaders/screen.json");
	shader_backdrop  = tau_gra_shader_make("data/shaders/backdrop.json");
	shader_loading   = tau_gra_shader_make("data/shaders/loading.json");
	font_default     = tau_gra_font_make("data/fonts/default.ttf", 32);
	framebuffer      = tau_gra_framebuffer_make(video.width, video.height);

	// Textures
	textures[texture_checkerboard] = tau_gra_texture_make("data/textures/checkerboard.png");
	textures[texture_wall]         = tau_gra_texture_make("data/textures/wall0.png");
	textures[texture_exclamation]  = tau_gra_texture_make("data/textures/warn.png");
	textures[texture_hudtimer]     = tau_gra_texture_make("data/textures/hud/timer.png");
}

#undef  TED_CURSUB
#define TED_CURSUB "g_world_start"
#include "entity_table.h"
void g_world_start(CTauCamera** newcamera)
{
	float aspectratio = (float)(video.width) / (float)(video.height);
	
	totaltime = 0.0f;
	hands = nullptr;
	g_world_load("world0", collisions, &thisworld);
	
	// The created camera should be where the player spawns.
	// Find the player spawner.
	for (ENTITY e: thisworld.entities)
	{
		switch(e.eid)
		{
			case EID::PLAYERSPAWN:
				TED_PRINT_INFO(std::to_string(e.angle));
				*newcamera = new CTauCamera((float) e.x, CAMERA_HEIGHT, (float) e.y, aspectratio);
				(*newcamera)->Turn(e.angle);
				(*newcamera)->Recalculate();
			default:
				break;
		}
	}
	if (!(*newcamera))
	{
		TED_PRINT_ERROR("There is no camera entity, \"EID=1\"!!!");
		*newcamera = new CTauCamera(0.0f, CAMERA_HEIGHT, 0.0f, aspectratio);
		(*newcamera)->Turn(0.0f);
		(*newcamera)->Recalculate();
	}
	
	tau_gra_shader_use(&shader_backdrop);
	tau_gra_shader_setuniformFlt3(&shader_backdrop, "colour_ceiling", thisworld.colours[0]);
	tau_gra_shader_setuniformFlt3(&shader_backdrop, "colour_ground",  thisworld.colours[1]);
}

static bool intersect(glm::vec2 p1, glm::vec2 p2, glm::vec2 q1, glm::vec2 q2)
{
    return (((q1.x-p1.x)*(p2.y-p1.y) - (q1.y-p1.y)*(p2.x-p1.x)) * ((q2.x-p1.x)*(p2.y-p1.y) - (q2.y-p1.y)*(p2.x-p1.x)) < 0) &&
           (((p1.x-q1.x)*(q2.y-q1.y) - (p1.y-q1.y)*(q2.x-q1.x)) * ((p2.x-q1.x)*(q2.y-q1.y) - (p2.y-q1.y)*(q2.x-q1.x)) < 0);
}

#undef  TED_CURSUB
#define TED_CURSUB "hands_drop"
void hands_drop(void)
{
	if (hands)
		(*hands).flags ^= E_FLAG_HANDSELECT;
	hands = nullptr;
}

#undef  TED_CURSUB
#define TED_CURSUB "entity_in_radius"
bool entity_in_radius(ENTITY e, CTauCamera* camera, float radius)
{
	
	float alpha         = camera->GetAngle();
	glm::vec3 camerapos = camera->GetPosition();
	glm::vec2 ray_pos0(camerapos.x, camerapos.z);
	glm::vec2 ray_pos1(camerapos.x + cos(alpha) * radius, camerapos.z + sin(alpha) * radius);
	glm::vec2 ent_pos0(e.x - (e.width / 2.0f) * cos(alpha + 3.1415f/2.0f), e.y - (e.width / 2.0f) * sin(alpha + 3.1415f/2.0f));
	glm::vec2 ent_pos1(e.x + (e.width / 2.0f) * cos(alpha + 3.1415f/2.0f), e.y + (e.width / 2.0f) * sin(alpha + 3.1415f/2.0f));
	return intersect(ray_pos0, ray_pos1, ent_pos0, ent_pos1);
}

#undef  TED_CURSUB
#define TED_CURSUB "hands_pickupnearest"
void hands_pickupnearest(CTauCamera* camera)
{
	const float MAX_PICKUPDIST = 14.0f;
	
	// Only certain entities can be picked up...
	// One is EID_CRATE, the crate entity.
	for (ENTITY& e: thisworld.entities)
	{
		if (e.eid != EID::CRATE)
			continue;
		if (entity_in_radius(e, camera, MAX_PICKUPDIST))
		{
			e.flags ^= E_FLAG_HANDSELECT;
			hands    = &e;
		}
	}
}

// Handle player movement
// Recalculate camera's matrices only if its state changes (moving, rotating...)
#define sgn(x) (x >= 0.0f ? 1.0f : -1.0)
#define RADIUS_COLLISION 64.0f
#undef  TED_CURSUB
#define TED_CURSUB "movement"
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
	if (keys[SDL_SCANCODE_SPACE] && !hands)
		hands_pickupnearest(*camera);
	else if (keys[SDL_SCANCODE_Q])
		hands_drop();

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
	for (unsigned int i = 0; i < collisions.size(); i++)
	{
		glm::vec4 wall      = collisions.at(i);
		glm::vec3 camerapos = (*camera)->GetPosition();
		glm::vec2 cam_p0    = glm::vec2(camerapos.x, camerapos.z);
		glm::vec2 cam_p1    = glm::vec2(camerapos.x + vecmove.x * RADIUS_COLLISION, camerapos.z + vecmove.y * RADIUS_COLLISION);
		glm::vec2 wall_p0   = glm::vec2(wall.x, wall.y);
		glm::vec2 wall_p1   = glm::vec2(wall.z, wall.w);
		float wallangle     = atan2(wall_p1.y - wall_p0.y, wall_p1.x - wall_p0.x);
		if (intersect(cam_p0, cam_p1, wall_p0, wall_p1))
		{
			float dx0 = cam_p0.x  - cam_p1.x;
			float dy0 = cam_p0.y  - cam_p1.y;
			float dx1 = wall_p0.x - wall_p1.x;
			float dy1 = wall_p0.y - wall_p1.y;
			float m0  = sqrt(dx0*dx0 + dy0*dy0);
			float m1  = sqrt(dx1*dx1 + dy1*dy1);
			float angle = acos((dx0*dx1 + dy0*dy1) / (m0 * m1));
#define sgn(x) (x >= 0.0f ? 1.0f : -1.0)
			vecmove.x = sgn(angle) * vecmove.x * abs(cos(wallangle));
			vecmove.y = sgn(angle) * vecmove.y * abs(sin(wallangle));
#undef sgn
		}
	}

	if (recalculate)
	{
		(*camera)->Move(vecmove.x, 0.0f, vecmove.y);
		(*camera)->Recalculate();
	}
}

#undef  TED_CURSUB
#define TED_CURSUB "g_world_tick"
void g_world_tick(CTauCamera* camera, float delta, int fps, const Uint8* keys, int mousedeltax, uint32_t frame)
{
	glm::mat4 identity        = glm::mat4(1.0f);
	totaltime += delta;

	movement(&camera, delta, keys, mousedeltax);

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
	for (model m: thisworld.bases)
	{
		MESH part       = m.first;
		TEXTURE texture = m.second;
		tau_gra_texture_use(&texture, TAU_TEXTUREUNIT_0);
		tau_gra_ren_mesh(&part);
	}

	// Entities
	for (ENTITY& e: thisworld.entities)
	{
		glm::mat4 model      = identity;
		MESH* mesh           = nullptr;
		float render_height  = e.height;
		float render_x       = e.x;
		float render_y       = e.y;
		float render_rotate  = 0.0f;
		
		if (e.flags & E_FLAG_HANDSELECT)
		{
			render_rotate  = camera->GetAngle();
			render_height += camera->GetPosition().y * (1.0/8.0);
			render_x       = camera->GetPosition().x + cos(render_rotate) * 5;
			render_y       = camera->GetPosition().z + sin(render_rotate) * 5;
			e.x = camera->GetPosition().x;
			e.y = camera->GetPosition().z;
		}
		else if ((e.flags & E_FLAG_FLASHCLOSE) && entity_in_radius(e, camera, 8))
			tau_gra_shader_setuniformFlt3(&shader_default, "tintcolor", glm::value_ptr(glm::vec3(1.0f, 0.75f + sin(frame/128.0f)/3.0f, 0.1f)));
		else
			tau_gra_shader_setuniformFlt3(&shader_default, "tintcolor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

		switch(e.eid)
		{
			default:
			case EID::PLAYERSPAWN:
				{
				glm::vec3 camerapos = camera->GetPosition();
				float dx = camerapos.x - e.x;
				float dy = camerapos.z - e.y;
				float angle = -atan2(dy, dx);
		
				mesh            = &mesh_panel;
				model           = glm::translate(model, glm::vec3((float)e.x, (float)e.height, (float)e.y));
				model           = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
				model           = glm::scale(model, glm::vec3(1.0, e.height, e.width));
				}
				break;
			case EID::CRATEINDENT:
				mesh            = &mesh_indent;
				model           = glm::translate(model, glm::vec3((float)e.x, 0.0f, (float)e.y));
				model           = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
				break;
			case EID::CRATE:
				mesh            = &mesh_crate;
				model           = glm::translate(model, glm::vec3(render_x, render_height, render_y));
				model           = glm::rotate(model, -render_rotate, glm::vec3(0.0f, 1.0f, 0.0f));
				model           = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
				break;
		}

		tau_gra_shader_setuniformMat4(&shader_default, "model", glm::value_ptr(model));
		tau_gra_texture_use(&e.texture, TAU_TEXTUREUNIT_0);
		tau_gra_ren_mesh(mesh);
	}

	// Make sure we're using the default tint value...
	tau_gra_shader_setuniformFlt3(&shader_default, "tintcolor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

	// Render to screen
	float aspectratio = (float)(video.width) / (float)(video.height);
	tau_gra_framebuffer_use(nullptr);
	tau_gra_clear(TAU_CLEAR_DEPTHBUFFER);
	tau_gra_shader_use(&shader_screen);
	tau_gra_shader_setuniformInt1(&shader_screen, "texture0", 0);
	tau_gra_shader_setuniformFlt2(&shader_screen, "scale2d", glm::value_ptr(glm::vec2(1.0f, 1.0f)));
	tau_gra_shader_setuniformFlt2(&shader_screen, "pos2d", glm::value_ptr(glm::vec2(0.0f, 0.0f)));
	tau_gra_shader_setuniformFlt2(&shader_screen, "uvscale", glm::value_ptr(glm::vec2(1.0f, 1.0f)));
	tau_gra_shader_setuniformInt1(&shader_screen, "istext", 0);
	tau_gra_texture_use(&framebuffer.attachedtexture, TAU_TEXTUREUNIT_0);
	tau_gra_ren_mesh_unitsquare();

	// HUD
	tau_gra_disableDepthTest();
	{
		glm::vec3 colourhud(1.0f, 1.0f, 1.0f);
		tau_gra_shader_setuniformFlt3(&shader_screen, "tintcolor", glm::value_ptr(colourhud));
		tau_gra_shader_setuniformFlt2(&shader_screen, "uvscale", glm::value_ptr(glm::vec2(1.0f, -1.0f)));
		
		// Timer
		tau_gra_shader_setuniformFlt2(&shader_screen, "scale2d", glm::value_ptr(glm::vec2(0.05f, 0.05f*aspectratio)));
		tau_gra_shader_setuniformFlt2(&shader_screen, "pos2d", glm::value_ptr(glm::vec2(-0.95f, -1.0f + 0.05*aspectratio)));
		tau_gra_texture_use(&textures[texture_hudtimer], TAU_TEXTUREUNIT_0);
		tau_gra_ren_mesh_unitsquare();
		tau_gra_shader_setuniformInt1(&shader_screen, "istext", 1);
		tau_gra_font_rendertext(&font_default, &shader_screen, std::to_string((int)floor(totaltime)) + "s; FPS:" + std::to_string(fps), 22, 0, 0.005f, aspectratio);
	}
	tau_gra_enableDepthTest();
}

void g_world_quit(void)
{
	tau_gra_framebuffer_destroy(&framebuffer);
	tau_gra_font_destroy(&font_default);
	tau_gra_shader_destroy(&shader_backdrop);
	tau_gra_shader_destroy(&shader_default);
	tau_gra_shader_destroy(&shader_screen);
	for (int i = 0; i < TEXTURES_PERSISTENT_SIZE; i++)
		tau_gra_texture_destroy(&textures[i]);
	tau_gra_mesh_delete(&mesh_panel);
	tau_gra_mesh_delete(&mesh_crate);
	tau_gra_mesh_delete(&mesh_indent);
	g_world_destroy(&thisworld);
}
