#include <sstream>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "world_load.h"
#include "../eng/io.h"
#include "../eng/print.h"

typedef struct
{
	int id;
	int x, y;
} VERTEX;

typedef struct
{
	int v0_id, v1_id;
	int id;
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

// Loading a world file as follows:
// 1. line defines amount of vertices, lines, tris and entities, IN ORDER!
//
// 2. line follows with vertex, line, tri and entitity data,
// datas are seperated by the ';' character.
//
// Example:
// 2:1:0:0
// 1, 0, 0; 2, 0, 4; 1, 2, 1;
// Here, there are two vertices and one line.
// First  vertex has data {1, 0, 0}, ID is 1, X is 0 and Y is 0.
// Second vertex has data {2, 0, 4}, ID is 2, X is 0 and Y is 4.
// First line has data {1, 2, 1}, ID is 1, V0_ID is 1, V1_ID is 2.
#undef  TED_CURSUB
#define TED_CURSUB "g_world_load"
void g_world_load(const char* name, std::vector<glm::vec4>& collisionlines, ENTITY* entities, int* entity_len)
{
	TED_PRINT_INFO("Loading world...");
	
	std::string path_worldinfo = std::string("data/worlds/") + name;
	std::string path_worldobj  = std::string("data/models/") + name + ".obj";
	
	std::istringstream data(tau_io_file_allcontent(path_worldinfo.c_str()));
	
	// Store first and second line
	std::string line0, line1;
	std::getline(data, line0);
	std::getline(data, line1);
	
	// Extract data lengths
	unsigned int i = 0, data_lengths[4], data_length = 0;
	size_t pos = 0;
	std::string token;
	while ((pos = line0.find(";")) != std::string::npos)
	{
		token           = line0.substr(0, pos);
		data_lengths[i] = std::stoi(token);
		data_length    += data_lengths[i];
		line0.erase(0, pos + 1);
		i++;
	}
	
	// Split the whole line into single data type segments,
	// split segments into values.
	// Count the current segment and value we are interpreting
	std::vector<VERTEX> data_vertices(data_lengths[0]);
	std::vector<LINE>   data_lines   (data_lengths[1]);
	std::vector<TRI>    data_tris    (data_lengths[2]);
	std::vector<ENTITY> data_entities(data_lengths[3]);
	std::stringstream   data_stream  (line1);
	std::string data_segment;
	unsigned int segmentIndex = 0;
	while (std::getline(data_stream, data_segment, ';'))
	{
		unsigned int valueIndex = 0;
		std::stringstream data_valuestream(data_segment);
		std::string data_value;
		while (std::getline(data_valuestream, data_value, ','))
		{
			int value = std::stoi(data_value);
			
			// Vertex segment
			if (segmentIndex < data_lengths[0])
			{
				if (valueIndex == 0) data_vertices[segmentIndex].id = value;
				if (valueIndex == 1) data_vertices[segmentIndex].x  = value;
				if (valueIndex == 2) data_vertices[segmentIndex].y  = value;
			}
			// Line segment
			else if (segmentIndex < data_lengths[0] + data_lengths[1])
			{
				unsigned int index = segmentIndex - data_lengths[0];
				if (valueIndex == 0) data_lines[index].v0_id  = value;
				if (valueIndex == 1) data_lines[index].v1_id  = value;
				if (valueIndex == 2) data_lines[index].id     = value;
			}
			// Tri segment
			else if (segmentIndex < data_lengths[0] + data_lengths[1] + data_lengths[2])
			{
				// TODO
			}
			// Entity segment
			else
			{
				unsigned int index = segmentIndex - data_lengths[0] - data_lengths[1] - data_lengths[2];
				if (valueIndex == 0) data_entities[index].x     = value;
				if (valueIndex == 1) data_entities[index].y     = value;
				if (valueIndex == 2) data_entities[index].id    = value;
				if (valueIndex == 3) data_entities[index].eid   = value;
				if (valueIndex == 4) data_entities[index].flags = value;
				if (valueIndex == 5) data_entities[index].angle = value;
			}
			valueIndex++;
		}
		segmentIndex++;
	}
	
	// Every line should be solid, have collision.
	// For every line, find and push corresponding vertex's position.
	for (i = 0; i < data_lines.size(); i++)
	{
		LINE l        = data_lines.at(i);
		VERTEX v0     = find_vertex(l.v0_id, data_vertices);
		VERTEX v1     = find_vertex(l.v1_id, data_vertices);
		glm::vec4 pos = glm::vec4(v0.x, v0.y, v1.x, v1.y);
		collisionlines.push_back(pos);
	}
	
	// Put entities in the array
	*entity_len = data_entities.size();
	for (i = 0; i < data_entities.size(); i++)
	{
		entities[i] = data_entities.at(i);
	}
}
