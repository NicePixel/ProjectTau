/* ogl_mesh.h
 * Load or delete 3D objects from files.
 */
#pragma once
#include <GL/glew.h>
#include <GL/GL.h>

// MESH structure 
// VBO is the vertex buffer object. 
// https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
// VAO is the vertex array object.
// totalvertices is the total amount of vertices for completely drawing the obj.
// https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
typedef struct _MESH
{
	GLuint vbo, vao, totalvertices;
} MESH;

void tau_gra_mesh_init(void);
void tau_gra_mesh_quit(void);

// Creates an instance of MESH data structure containing the created buffers
// for the loaded .obj file with the given path.
// Also holds the total amount of vertices the object is made of, this is useful
// for rendering the loaded .obj.
//
// !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!
// The object in .obj file HAS TO BE TRIANGULATED.
// The object in .obj file HAS TO BE UNWRAPPED FOR TEXTURING.
// !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!   !!!
//
// The .obj loader is good enough for the simple job it does. Do not feed it 
// enormous files though.
//
// Will throw CTauGraException if things go wrong.
MESH tau_gra_mesh_make(const char* filepath);
void tau_gra_mesh_delete(MESH* mesh);

// Default square buffers which are used primarly for drawing text charcaters.
// They are initialized in `tau_gra_mesh_init` and should NOT be used with any
// other shader BUT the text drawing shader.
// Calling `tau_gra_mesh_quit` will delete these buffers.
//
// They are fundamentally created differently.
// Each vertex has 4 defining floats (x, y, u, v), the coordinates in the world
// space and the coordinate for texturing.
//
// This is the best idea I had for text rendering buffers.
GLuint tau_gra_mesh_getsquareVAO(void);
GLuint tau_gra_mesh_getsquareVBO(void);
