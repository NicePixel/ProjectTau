#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "entity.h"

void g_world_load(const char* name, std::vector<glm::vec4>& collisionlines, ENTITY* entities, int* entity_len);
