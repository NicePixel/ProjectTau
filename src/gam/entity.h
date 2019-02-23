#pragma once

#include "../eng/r_texture.h"

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
