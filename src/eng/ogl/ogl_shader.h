#pragma once
#include "../global.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include <map>
#include <vector>

typedef std::map<std::string, GLint> mapuniform;
typedef struct _SHADER
{
	GLuint program;
	mapuniform m_uniforms;
} SHADER;

SHADER tau_gra_shader_make          (const char* fileVertex, const char* fileFragment);
void   tau_gra_shader_destroy       (SHADER* sha);
void   tau_gra_shader_use           (SHADER* sha);

// Set a uniform in shader.
void   tau_gra_shader_setuniformMat4(SHADER* sha, const char* uniform, const GLfloat *value);
void   tau_gra_shader_setuniformInt1(SHADER* sha, const char* uniform, const GLint value);
void   tau_gra_shader_setuniformFlt1(SHADER* sha, const char* uniform, const GLfloat value);
void   tau_gra_shader_setuniformFlt3(SHADER* sha, const char* uniform, const GLfloat* value);
