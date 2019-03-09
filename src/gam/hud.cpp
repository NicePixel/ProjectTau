#include <glm/glm.hpp>
#include "../eng/render.h"
#include "../eng/print.h"
#include "hud.h"
#include "texture_table.h"

static float ratio       = 1.0f;

void g_hud_init(float aspectratio)
{
	ratio = aspectratio;
}

void g_hud_tick(float totaltime, float fps, bool hands, bool canpickup, TEXTURE* textures, FONT* font, SHADER* sha)
{
	const glm::vec3 flashingcolour(0.2f, 0.75f + sin(totaltime * 10.0)/4.0f, 0.2f);
	
	tau_gra_disableDepthTest();
	{
		tau_gra_shader_setuniformFlt3(sha, "tintcolor", HUD_TINT);
		tau_gra_shader_setuniformFlt2(sha, "uvscale", glm::value_ptr(glm::vec2(1.0f, -1.0f)));
		
		// Timer
		tau_gra_shader_setuniformFlt2(sha, "scale2d", glm::value_ptr(glm::vec2(0.05f, 0.05f*ratio)));
		tau_gra_shader_setuniformFlt2(sha, "pos2d", glm::value_ptr(glm::vec2(-0.95f, -1.0f + 0.05*ratio)));
		tau_gra_texture_use(&textures[texture_hudtimer], TAU_TEXTUREUNIT_0);
		tau_gra_ren_mesh_unitsquare();
		tau_gra_shader_setuniformInt1(sha, "istext", 1);
		tau_gra_font_rendertext(font, sha, std::to_string((int)floor(totaltime)) + "s; FPS:" + std::to_string(fps), 22, 0, 0.005f, ratio);
		
		// Hand (if close to an entity or holding one)
		if (canpickup || hands)
		{
			if (hands)
				tau_gra_shader_setuniformFlt3(sha, "tintcolor", glm::value_ptr(flashingcolour));
			tau_gra_shader_setuniformInt1(sha, "istext", 0);
			tau_gra_shader_setuniformFlt2(sha, "scale2d", glm::value_ptr(glm::vec2(0.05f, 0.05f*ratio)));
			tau_gra_shader_setuniformFlt2(sha, "pos2d", glm::value_ptr(glm::vec2(0.0f, -1.0f + 0.05*ratio)));
			tau_gra_texture_use(&textures[texture_hudhand], TAU_TEXTUREUNIT_0);
			tau_gra_ren_mesh_unitsquare();
			if (hands)
				tau_gra_shader_setuniformFlt3(sha, "tintcolor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		}
	}
	tau_gra_enableDepthTest();
}
