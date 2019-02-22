#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "entity.h"
#include "../eng/r_mesh.h"
#include "../eng/r_texture.h"

typedef std::pair<MESH, TEXTURE> model;
typedef struct
{
	std::vector<model> bases;
} WORLD;

void g_world_load(const char* name, std::vector<glm::vec4>& collisionlines, WORLD* world);
void g_world_destroy(WORLD* world);