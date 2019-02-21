#include "ogl_mesh.h"
#include "../io.h"
#include "../print.h"
#include "ogl_exception.h"
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../timer.h"
#include <stdexcept>

static GLuint VBOsqaure, VAOsquare;

void tau_gra_mesh_init(void)
{
	GLfloat vertices[6][4] = {
			   { -1.0, -1.0,   0.0, 0.0 },
			   { 1.0,  -1.0,       1.0, 0.0 },
			   { 1.0, 1.0,       1.0, 1.0 },

			   { 1.0, 1.0,   1.0, 1.0 },
			   { -1.0, 1.0,       0.0, 1.0 },
			   { -1.0, -1.0,   0.0, 0.0 }
	};

	// Create buffers
	glGenVertexArrays(1, &VAOsquare);
	glGenBuffers(1, &VBOsqaure);
	glBindVertexArray(VAOsquare);
	glBindBuffer(GL_ARRAY_BUFFER, VBOsqaure);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void tau_gra_mesh_quit(void)
{
	glDeleteBuffers(1, &VBOsqaure);
	glDeleteVertexArrays(1, &VAOsquare);
}

GLuint tau_gra_mesh_getsquareVAO(void)
{
	return VAOsquare;
}

GLuint tau_gra_mesh_getsquareVBO(void)
{
	return VBOsqaure;
}

#undef  TED_CURSUB
#define TED_CURSUB "loadobj"
void loadobj(GLuint* outVBO, GLuint* outVAO, const char* objfile, GLuint* outTotalVertices)
{
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::vector<GLfloat> datacombined;
	std::string line;
	GLuint countVertices = 0;
	int lineCount = 1;

	// Input file
	std::ifstream infile(objfile);
	if (!infile.good())
	{
		TED_PRINT_ERROR       ("Can't open file for reading!");
		throw CTauGraException("Can't open file for reading!");
	}
	
	// Read line by line, splitting every line into tokens.
	while (std::getline(infile, line))
	{
		const char* cline = line.c_str();
		char* token;
		std::vector<std::string> tokens;

		token = strtok((char*)cline, " ");
		while (token)
		{
			tokens.push_back(std::string(token));
			token = strtok(0, " ");
		}

		// Empty line?
		if (tokens.empty())
			continue;

		unsigned int tokensSize = tokens.size();

		// Vertices
		if (tokens.at(0) == "v")
		{
			if (tokensSize < 4)
			{
				TED_PRINT_INFO("Abnormal length of line " + std::to_string(lineCount) + ". Can't proceed.");
				throw CTauGraException(".obj has an invalid line.");
			}
			else
			{
				glm::vec3 vertex;
				if (tokensSize > 4)
					TED_PRINT_INFO("Abnormal length of line " + std::to_string(lineCount));
				vertex.x = stof(tokens.at(1));
				vertex.y = stof(tokens.at(2));
				vertex.z = stof(tokens.at(3));
				temp_vertices.push_back(vertex);
			}
		}

		// UV
		else if (tokens.at(0) == "vt") // or tokens.at(0) == "vt"
		{
			if (tokensSize < 3)
			{
				TED_PRINT_INFO("Abnormal length of line " + std::to_string(lineCount) + ". Can't proceed.");
				throw CTauGraException(".obj has an invalid line.");
			}
			else
			{
				glm::vec2 uv;
				if (tokensSize > 3)
					TED_PRINT_INFO("Abnormal length of line " + std::to_string(lineCount));
				uv.x = stof(tokens.at(1));
				uv.y = stof(tokens.at(2));
				temp_uvs.push_back(uv);
			}
		}

		// Normals
		else if (tokens.at(0) == "vn")
		{
			if (tokens.size() < 4)
			{
				TED_PRINT_INFO("Abnormal length of line " + std::to_string(lineCount) + ". Can't proceed.");
				throw CTauGraException(".obj has an invalid line.");
			}
			else
			{
				glm::vec3 normal;
				if (tokensSize > 4)
					TED_PRINT_INFO("Abnormal length of line " + std::to_string(lineCount));
				normal.x = stof(tokens.at(1));
				normal.y = stof(tokens.at(2));
				normal.z = stof(tokens.at(3));
				temp_normals.push_back(normal);
			}
		}

		// Face
		else if (tokens.at(0) == "f")
		{
			// Face line is defined as (for example)
			// f 126/185/55 127/186/55 119/174/55
			// We tokenize '126/185/55', '127/186/55' and '119/174/55' with delimeter '/'.
			// After tokenization we convert every token into an integer and push it to respective vector.
			for (int i = 0; i < 3; i++)
			{
				std::vector<std::string> tokens_face(3);
				const char* cword = tokens.at(1 + i).c_str();
				char* token = strtok((char*)cword, "/");
				int j = 0;
				while (token)
				{
					if (j < 3)
						tokens_face[j] = std::string(token);
					token = strtok(0, "/");
					j++;
				}
				if (j != 3)
				{
					TED_PRINT_ERROR(std::string("Face doesn't have EXACTLY 3 defining indices. This file can't be loaded. Line ") + std::to_string(lineCount));
					throw CTauGraException(".obj face does not have EXACTLY 3 defining indices.");
				}

				int indexVertex = stoi(tokens_face.at(0));
				int indexUv     = stoi(tokens_face.at(1));
				int indexNormal = stoi(tokens_face.at(2));

				glm::vec3 v = temp_vertices[indexVertex - 1];
				glm::vec2 u = temp_uvs     [indexUv     - 1];
				glm::vec3 n = temp_normals [indexNormal - 1];

				datacombined.push_back(v.x);
				datacombined.push_back(v.y);
				datacombined.push_back(v.z);
				datacombined.push_back(u.x);
				datacombined.push_back(u.y);
				datacombined.push_back(n.x);
				datacombined.push_back(n.y);
				datacombined.push_back(n.z);
				
			}
			countVertices += 3;

		}

		lineCount++;
	}

	*outTotalVertices = countVertices;

	// Set up VAO
	glGenVertexArrays(1, outVAO);
	glBindVertexArray(*outVAO);

	// Set up VBO
	glGenBuffers(1, outVBO);
	glBindBuffer(GL_ARRAY_BUFFER, *outVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBufferData(GL_ARRAY_BUFFER, datacombined.size() * sizeof(GLfloat), &datacombined[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

MESH tau_gra_mesh_make(std::vector<float> bufferdata)
{
	MESH m;
	
	// Because the data format is predetermined, we know there is going to be 
	// datalength/8 vertices
	m.totalvertices = bufferdata.size() / 8;
	
	// Set up VAO
	glGenVertexArrays(1, &m.vao);
	glBindVertexArray(m.vao);

	// Set up VBO
	glGenBuffers(1, &m.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBufferData(GL_ARRAY_BUFFER, bufferdata.size() * sizeof(GLfloat), &bufferdata[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	return m;
}

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_mesh_make"
MESH tau_gra_mesh_make(const char* filepath)
{
	MESH m;
	TED_PRINT_INFO(std::string("Loading ") + filepath);
	tau_timer_start();
	loadobj(&m.vbo, &m.vao, filepath, &m.totalvertices);
	TED_PRINT_INFO(std::string("\tTime taken [s]: ") + std::to_string(tau_timer_stop()));
	TED_PRINT_INFO(std::string("\tVertex amount : ") + std::to_string(m.totalvertices));
	return m;
}

void tau_gra_mesh_delete(MESH* mesh)
{
	glDeleteBuffers(1, &mesh->vao);
	glDeleteBuffers(1, &mesh->vbo);
}
