#pragma once

#include "../eng/r_texture.h"

#define E_FLAG_HANDSELECT 1
typedef struct
{
	int   x, y;
	int   width, height;
	int   id;
	int   eid;
	int   flags;
	float angle;
	TEXTURE texture;
} ENTITY;
