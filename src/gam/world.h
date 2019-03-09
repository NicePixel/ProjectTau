#pragma once
#include <stdint.h>

// Shaders are persistent data, initialized within `g_world_start_persistent()`
// These defines shorten typing of the shaders
// The index values are used in getting the correct shader out of world.cpp
#define shader_default          shaders[0]
#define shader_text             shaders[1]
#define shader_screen           shaders[2]
#define shader_backdrop         shaders[3]
#define shader_loading          shaders[4]
#define shader_default_index    0
#define shader_text_index       1
#define shader_screen_index     2
#define shader_backdrop_index   3
#define shader_loading_index    4

// For the textures array...
#include "texture_table.h"

#include "../eng/render.h"
void g_world_loadscreen(void);
void g_world_start_persistent(void);
void g_world_start(CTauCamera** newcamera);
void g_world_tick(CTauCamera* camera, float delta, int fps, const Uint8* keys, int mousedeltax, uint32_t frame);
void g_world_quit(void);
