#pragma once

static const float HUD_TINT[3] = {0.8f, 0.8f, 0.8f};

void g_hud_init(float aspectratio);
void g_hud_tick(float totaltime, float fps, bool hands, bool canpickup, TEXTURE* textures, FONT* font, SHADER* sha);
