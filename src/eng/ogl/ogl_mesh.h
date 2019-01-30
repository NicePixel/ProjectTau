#pragma once
#include <GL/glew.h>
#include <GL/GL.h>

typedef struct _MESH
{
	GLuint vbo, vao, totalvertices;
} MESH;

void tau_gra_mesh_init(void);
void tau_gra_mesh_quit(void);
MESH tau_gra_mesh_make(const char* filepath);
void tau_gra_mesh_delete(MESH* mesh);
GLuint tau_gra_mesh_getsquareVAO(void);
GLuint tau_gra_mesh_getsquareVBO(void);
