#pragma once

void g_world_start(CTauCamera** newcamera);
void g_world_tick(CTauCamera* camera, float delta, int fps, const Uint8* keys, int mousedeltax);
void g_world_quit(void);
