#include <sstream>
#include <vector>
#include <utility>
#include <iostream>
#include <glm/glm.hpp>
#include "world_load.h"
#include "../eng/io.h"
#include "../eng/print.h"
#include "../eng/configuration.h"

typedef struct
{
	int id;
	int x, y;
} VERTEX;

typedef struct
{
	int type;
	int x0, y0, x1, y1;
} LINE;

typedef struct
{
	int v0_id, v1_id, v2_id;
	int id;
} TRI;

// Entity structure type
#include "entity.h"

VERTEX find_vertex(int id, std::vector<VERTEX>& vertices)
{
	VERTEX v;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		if (vertices.at(i).id == id)
		{
			return vertices.at(i);
		}
	}
	return v;
}

#undef  TED_CURSUB
#define TED_CURSUB "g_world_load"
static const float SCALE  = 16.0f;
static const float HEIGHT = 16.0f;
void g_world_load(const char* name, std::vector<glm::vec4>& collisionlines, WORLD* world)
{
	std::string worldpath = std::string("data/worlds/") + name + ".json";
	TED_PRINT_INFO(std::string("Begin world ") + worldpath);

	nlohmann::json config = tau_config_load(worldpath.c_str());
	nlohmann::json data   = config["data"];
	nlohmann::json types  = config["types"];
	int typecount         = config["type-count"];
	std::vector<std::vector<glm::vec4> > walldata(typecount);
	
	int buildwall_type, data_index = 0;
	glm::vec4 buildwall;
	for (nlohmann::json::iterator it = data.begin(); it != data.end(); ++it)
	{
		nlohmann::json wall = *it;

		// Wall type
		if ((data_index % 3) == 0)
		{
			buildwall_type = (int) wall;
		}
		// First coordinate
		else if ((data_index % 3) == 1)
		{
			glm::vec4 coords;
			buildwall.x = (float) wall[0] * SCALE;
			buildwall.y = (float) wall[1] * HEIGHT;
		}
		// Second coordinate
		// This is last parameter we need to make a wall
		else if ((data_index % 3) == 2)
		{
			buildwall.z = (float) wall[0] * SCALE;
			buildwall.w = (float) wall[1] * HEIGHT;
			
			TED_PRINT_INFO(std::string("Push wall... +") + std::to_string(buildwall_type));
			walldata[buildwall_type].push_back(buildwall);
			collisionlines          .push_back(buildwall);
		}

		data_index++;
	}

	for (int i = 0; i < typecount; i++)
	{
		std::vector<float> bufferdata;
		std::vector<glm::vec4> walls = walldata.at(i);
		for (unsigned int j = 0; j < walls.size(); j++)
		{
			glm::vec4 wall = walls.at(j);
			float topleft[8],  topright[8], bottomleft[8], bottomright[8];
			float dx = (wall.x - wall.z) / SCALE;
			float dy = (wall.y - wall.w) / HEIGHT;
			float d  = sqrt(dx*dx + dy*dy);

			// Buffer data format:
			// Vx Vy Vz Ux Uy Nx Ny Nz
			float nx = -dy;
			float ny = dx;
			topleft[0] = wall.x; /* Vx */
			topleft[1] = HEIGHT; /* Vy */
			topleft[2] = wall.y; /* Vz */
			topleft[3] = 0.0f;   /* Ux */
			topleft[4] = 1.0f;   /* Uy */
			topleft[5] = nx;     /* Nx */
			topleft[6] = 0.0f;   /* Ny */
			topleft[7] = ny;     /* Nz */
			
			topright[0] = wall.z;
			topright[1] = HEIGHT;
			topright[2] = wall.w;
			topright[3] = d;
			topright[4] = 1.0f;
			topright[5] = nx;
			topright[6] = 0.0f;
			topright[7] = ny;
			
			bottomleft[0] = wall.x;
			bottomleft[1] = 0.0f;
			bottomleft[2] = wall.y;
			bottomleft[3] = 0.0f;
			bottomleft[4] = 0.0f;
			bottomleft[5] = nx;
			bottomleft[6] = 0.0f;
			bottomleft[7] = ny;
			
			bottomright[0] = wall.z;
			bottomright[1] = 0.0f;
			bottomright[2] = wall.w;
			bottomright[3] = d;
			bottomright[4] = 0.0f;
			bottomright[5] = nx;
			bottomright[6] = 0.0f;
			bottomright[7] = ny;
			
#define PUSH(array) for (int i = 0; i < 8; i++) { bufferdata.push_back(array[i]); }
			PUSH(bottomright);//\///////////////////////////\\\////|
			PUSH(topright);//"`\\/'    .   .             ,, `\\//'||
			PUSH(topleft);//    ` ,               .  .  <==:  `'  \|
			//                   :=>      ,...           ``       #
			PUSH(topleft);//\.    `    .////\\\\.                 /|
			PUSH(bottomleft);//\,   .////////\\\\\                ||
			PUSH(bottomright);///////////////\\\\\\///////////////\/
#undef PUSH
		}
		
		// Create a model consisting of a mesh and a texture
		// Mesh is created from bufferdata, and texture is created from the
		// appropriate texture's file path the wall's type uses.
		std::string texturepath = types[i]["texture"];
		model mdl;
		mdl.first  = tau_gra_mesh_make(bufferdata);
		mdl.second = tau_gra_texture_make(texturepath.c_str());
		world->bases.push_back(mdl);
	}
	
}

void g_world_destroy(WORLD* world)
{
	for (model m: world->bases)
	{
		tau_gra_mesh_delete    (&m.first);
		tau_gra_texture_destroy(&m.second);
	}
	world->bases.clear();
}