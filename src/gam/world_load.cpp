#include <vector>
#include <utility>
#include <iostream>
#include <glm/glm.hpp>
#include "../eng/io.h"
#include "../eng/print.h"
#include "../eng/configuration.h"
#include "world_load.h"
#include "entity.h"

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

#undef  TED_CURSUB
#define TED_CURSUB "g_world_load"
static const float SCALE  = 16.0f;
static const float HEIGHT = 16.0f;
void g_world_load(const char* name, std::vector<glm::vec4>& collisionlines, WORLD* world)
{
	std::string worldpath = std::string("data/worlds/") + name + ".json";
	TED_PRINT_INFO(std::string("Begin world ") + worldpath);

	nlohmann::json config   = tau_config_load(worldpath.c_str());
	nlohmann::json data     = config["data"];
	nlohmann::json types    = config["types"];
	nlohmann::json textures = config["textures"];
	int typecount           = config["type-count"];
	std::vector<std::vector<glm::vec4> > walldata(typecount);
	std::vector<TEXTURE> texturedata(textures.size());
	
	// Load textures
	{
		unsigned int i = 0;
		for (nlohmann::json::iterator it = textures.begin(); it != textures.end(); ++it)
		{
			std::string texturepath = *it;
			texturedata[i++] = tau_gra_texture_make(texturepath.c_str());
		}
	}
	
	
	// Fill wall data
	{
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
				buildwall.y = (float) wall[1] * SCALE;
			}
			// Second coordinate
			// This is last parameter we need to make a wall
			else if ((data_index % 3) == 2)
			{
				buildwall.z = (float) wall[0] * SCALE;
				buildwall.w = (float) wall[1] * SCALE;
				
				walldata[buildwall_type].push_back(buildwall);
				collisionlines          .push_back(buildwall);
			}

			data_index++;
		}
	}

	// Every type of wall has its own mesh.
	// For each wall in every type, gather coordinates.
	// After gathering, create a mesh and a texture.
	for (int i = 0; i < typecount; i++)
	{
		//std::string texturepath = types[i]["texture"];
		std::vector<float> bufferdata;
		std::vector<glm::vec4> walls = walldata.at(i);
		for (unsigned int j = 0; j < walls.size(); j++)
		{
			glm::vec4 wall = walls.at(j);
			float topleft[8],  topright[8], bottomleft[8], bottomright[8];
			float dx = (wall.x - wall.z) / SCALE;
			float dy = (wall.y - wall.w) / SCALE;
			float d  = sqrt(dx*dx + dy*dy);
			float nx = -dy;
			float ny = dx;

			// Buffer data format:
			// Vx Vy Vz Ux Uy Nx Ny Nz
#define FILLARR(arr, Vx, Vy, Vz, Ux, Uy, Nx, Ny, Nz) \
arr[0] = Vx; arr[1] = Vy; arr[2] = Vz;\
arr[3] = Ux; arr[4] = Uy;\
arr[5] = Nx; arr[6] = Ny; arr[7] = Nz;
			FILLARR(topleft,     wall.x, HEIGHT, wall.y, 0.0f, 1.0f, nx, 0.0f, ny);
			FILLARR(topright,    wall.z, HEIGHT, wall.w, d,    1.0f, nx, 0.0f, ny);
			FILLARR(bottomleft,  wall.x, 0.0f,   wall.y, 0.0f, 0.0f, nx, 0.0f, ny);
			FILLARR(bottomright, wall.z, 0.0f,   wall.w, d,    0.0f, nx, 0.0f, ny);
#undef FILLARR
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
		model mdl;
		mdl.first  = tau_gra_mesh_make(bufferdata);
		mdl.second = texturedata[i]; //tau_gra_texture_make(texturepath.c_str());
		world->bases.push_back(mdl);
	}
	
	// Load entities
	{
		nlohmann::json entities = config["entities"];
		for (nlohmann::json::iterator it = entities.begin(); it != entities.end(); ++it)
		{
			nlohmann::json edata = *it;
			ENTITY e;
			e.eid     = (enum EID) edata[0];
			e.id      = (int)      edata[1];
			e.x       = (float)    edata[2][0];
			e.y       = (float)    edata[2][1];
			e.width   = (float)    edata[3][0];
			e.height  = (float)    edata[3][1];
			e.angle   = (float)    edata[4];
			e.flags   = (int)      0;
			e.texture = texturedata[edata[5]];
			world->entities.push_back(e);
		}
	}
	
	// Load colours
	{
		nlohmann::json colours = config["colours"];
#define LOADCOLOUR(arr, what) \
arr[0] = colours[what][0]; arr[1] = colours[what][1]; arr[2] = colours[what][2];
		LOADCOLOUR(world->colours[0], "ceiling");
		LOADCOLOUR(world->colours[1], "ground");
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
