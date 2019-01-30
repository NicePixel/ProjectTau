#include <GL/glew.h>
#include <GL/GL.h>
#include "ogl_render.h"

void tau_gra_ren_mesh(MESH* mesh)
{
	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * (3 + 2 + 3), (void*)(0 * sizeof(GLfloat)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * (3 + 2 + 3), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * (3 + 2 + 3), (void*)(5 * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLES, 0, mesh->totalvertices);
	glBindVertexArray(0);
}

void tau_gra_ren_mesh_unitsquare(void)
{
	glBindVertexArray(tau_gra_mesh_getsquareVAO());
	glBindBuffer(GL_ARRAY_BUFFER, tau_gra_mesh_getsquareVBO());
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6); // Square is comprised of 2 triangles.
	glBindVertexArray(0);
}
