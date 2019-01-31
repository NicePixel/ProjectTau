/* ogl_shader.h 
 * Shader manipulation.
 */
#pragma once
#include "../global.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include <map>
#include <vector>

// Every shader has a program ID assigned to it.
// Calling `tau_gra_shader_use` will make sure the subsequent render calls use
// the wanted shader.
//
// Manipulate the execution of the shaders by using and updating the shader's
// uniform values via `tau_gra_shader_setuniform*` functions.
// The SHADER struct type has a map of all known uniforms. The map's key is 
// the uniform ID used to write or read the value of the uniform.
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
