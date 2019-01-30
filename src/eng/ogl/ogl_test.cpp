#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <glm/gtc/type_ptr.hpp>
#include "ogl_exception.h"
#include "ogl_shader.h"
#include "ogl_mesh.h"
#include "ogl_texture.h"
#include "ogl_main.h"
#include "ogl_render.h"
#include "../object.h"
#include "../camera.h"
#include "../print.h"
#include "../timer.h"

static void test1(void);
static void test2(void);

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_test_runall"
void tau_gra_test_runall(void)
{
	TED_PRINT_INFO("Running graphics tests...");
	TED_PRINT_INFO("-------------------------");
	double time_total = 0.0;
	double time_this = 0.0;

	try
	{
		TED_PRINT_INFO("Test 1: clear screen");
		tau_timer_start();
		test1();
		time_this   = tau_timer_stop();
		time_total += time_this;
		TED_PRINT_INFO(std::string("Test 1 took ") + std::to_string(time_this) + "s");

		TED_PRINT_INFO("Test 2: models");
		tau_timer_start();
		test2();
		time_this = tau_timer_stop();
		time_total += time_this;
		TED_PRINT_INFO(std::string("Test 2 took ") + std::to_string(time_this) + "s");
	}
	catch(CTauGraException e)
	{
		TED_PRINT_ERROR("Test failed. CTauGraException was thrown.");
		TED_PRINT_ERROR(e.what());
		return;
	}

	TED_PRINT_INFO("Tests ended.");
	TED_PRINT_INFO(std::string("Total time elapsed during tests: ") + std::to_string(time_total) + "s");
	TED_PRINT_INFO("-------------------------");
}

// First test.
// Blue scren.
void test1(void)
{
	glClearColor(0.0, 0.0, 1.0, 1.0);
	tau_gra_clear(TAU_CLEAR_COLORANDDEPTHBUFFER);
	tau_gra_updatewindow();
	SDL_Delay(1000);
}

// Second test.
// Models.
void test2(void)
{
	SHADER sha_default = tau_gra_shader_make("data/shaders/test_vertex.glsl", "data/shaders/test_fragment.glsl");
	SHADER sha_texture = tau_gra_shader_make("data/shaders/test_vertex_tex.glsl", "data/shaders/test_fragment_tex.glsl");
	TEXTURE tex_wyvern = tau_gra_texture_make("data/textures/llvm.png");
	MESH mesh_torus  = tau_gra_mesh_make("data/models/torus.obj");
	MESH mesh_rabbit = tau_gra_mesh_make("data/models/torus.obj");
	MESH mesh_cube   = tau_gra_mesh_make("data/models/torus.obj");
	CTauCamera     camera(4.0, 1.0, 4.0);

	// Enable states
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glActiveTexture(GL_TEXTURE0);
	glClearColor(0.0, 0.0, 1.0, 1.0);

	for (unsigned int i = 0; i < 3; i++)
	{
		glm::mat4 mat4Model        = glm::mat4(1.0f);
		MESH* mesh;
		SHADER* usingshader = nullptr;
		switch (i)
		{
			default:
			case 0:
				mesh = &mesh_torus;
				usingshader = &sha_default;
				break;
			case 1:
				mesh = &mesh_rabbit;
				usingshader = &sha_default;
				break;
			case 2:
				mesh = &mesh_cube;
				usingshader = &sha_texture;
				break;
		}

		tau_gra_shader_use(usingshader);
		tau_gra_shader_setuniformInt1(usingshader, "texture0", 0);
		tau_gra_shader_setuniformMat4(usingshader, "proj", camera.GetValueProjection());
		tau_gra_shader_setuniformMat4(usingshader, "model", glm::value_ptr(mat4Model));
		tau_gra_texture_use(&tex_wyvern, TAU_TEXTUREUNIT_0);

		// Render
		for (int i = 0; i < 360 * 1; i++)
		{
			float rad = i * (3.1415f / 180.0f);
			camera.SetPosition(3.0f * sin(rad), (5.0f + sin(rad)) / 2.0f, 3.0f * cos(rad));
			camera.ForceLook(0.0f, 0.0f, 0.0f);
			tau_gra_shader_setuniformMat4(usingshader, "view", camera.GetValueView());

			tau_gra_clear(TAU_CLEAR_COLORANDDEPTHBUFFER);
			tau_gra_ren_mesh(mesh);

			tau_gra_updatewindow();
			SDL_Delay(10);
			SDL_PumpEvents();
		}

		tau_gra_mesh_delete(&mesh_torus);
		tau_gra_mesh_delete(&mesh_rabbit);
		tau_gra_mesh_delete(&mesh_cube);
		tau_gra_texture_destroy(&tex_wyvern);
		tau_gra_shader_destroy(&sha_default);
		tau_gra_shader_destroy(&sha_texture);
		tau_gra_texture_unuse();
		delete(mesh);
	}
}
