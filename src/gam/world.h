#pragma once
#include <stdint.h>

void g_world_start(CTauCamera** newcamera);
void g_world_tick(CTauCamera* camera, float delta, int fps, const Uint8* keys, int mousedeltax, uint32_t frame);
void g_world_quit(void);
