#pragma once

#include "../eng/r_texture.h"
#include "entity_table.h"

#define E_FLAG_HANDSELECT 1
typedef struct
{
	int      x, y;
	int      width, height;
	int      id;
	enum EID eid;
	int      flags;
	float    angle;
	TEXTURE texture;
} ENTITY;
