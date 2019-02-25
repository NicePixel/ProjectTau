#include <stdlib.h>
#include <string.h>
#include <list>
#include "ogl_exception.h"
#include "ogl_shader.h"
#include "../print.h"
#include "../io.h"
#include "../configuration.h"

static GLuint initshader(const std::string& shaderSource, GLenum shaderType);
static int    setuniform(GLint* outUniform, GLuint program, const char* name);

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_shader_make"
SHADER tau_gra_shader_make(const char* fileshader)
{
	TED_PRINT_INFO(std::string("Creating shader from JSON ") + fileshader);
	nlohmann::json config = tau_config_load(fileshader);
	std::string filevertex   = config["vertex"];
	std::string filefragment = config["fragment"];

	SHADER sha;
	std::string strVertex   = tau_io_file_allcontent(filevertex  .c_str());
	std::string strFragment = tau_io_file_allcontent(filefragment.c_str());


	// If something goes wrong with shader initalization,
	// an exception will be thrown.
	GLuint vtx = initshader(strVertex, GL_VERTEX_SHADER);
	GLuint frg = initshader(strFragment, GL_FRAGMENT_SHADER);
	GLuint prg = glCreateProgram();
	{
		GLint err;
		glAttachShader(prg, vtx);
		glAttachShader(prg, frg);
		glLinkProgram(prg);
		glGetProgramiv(prg, GL_LINK_STATUS, &err);
		if (!err)
		{
			glDeleteProgram(prg);
			throw CTauGraException("Shader program linking error.");
		}
	}

	// Set up shader struct
	sha.program = prg;

	// Set up uniforms
	std::list<std::string> uniforms
	{
		"proj", "view", "model", "pos2d", "scale2d",
		"texture0", "texture1",
		"uvscale",
		"tintcolor", "onlycolor", "rentype", "istext",
		"totaltime", "colour_ceiling", "colour_ground"
	};
	for (std::string uni: uniforms)
	{
		std::pair<std::string, GLint> unfirompair;
		unfirompair.first = uni;
		setuniform(&unfirompair.second, prg, unfirompair.first.c_str());
		sha.m_uniforms.insert(unfirompair);
	}
	
	// Delete unused
	glDetachShader(prg, vtx);
	glDetachShader(prg, frg);
	glDeleteShader(vtx);
	glDeleteShader(frg);

	return sha;
}

// By documentation, if a uniform doesn't exist OGL should return -1.
// Here we return the ID of -1 if there is no uniform with given name.
// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetUniformLocation.xhtml
#undef  TED_CURSUB
#define TED_CURSUB "getUniform"
static GLint getUniform(mapuniform map, const char* uniform)
{
	auto it = map.find(uniform);
	if (it != map.end())
		return it->second;
	TED_PRINT_INFO(std::string("Cant find uniform in map. Uniform is named: ") + uniform);
	return -1;
}

// Set uniforms with given type
#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_shader_setuniform*"
#define GET_AND_CHECK_UNIFORM GLint id = getUniform(sha->m_uniforms, uniform); if (id < 0) { return; }
void tau_gra_shader_setuniformMat4(SHADER* sha, const char* uniform, const GLfloat *value)
{
	GET_AND_CHECK_UNIFORM;
	glUniformMatrix4fv(id, 1, GL_FALSE, value);
}

void tau_gra_shader_setuniformInt1(SHADER* sha, const char* uniform, const GLint value)
{
	GET_AND_CHECK_UNIFORM;
	glUniform1i(id, value);
}

void tau_gra_shader_setuniformFlt1(SHADER* sha, const char* uniform, const GLfloat value)
{
	GET_AND_CHECK_UNIFORM;
	glUniform1f(id, value);
}

void tau_gra_shader_setuniformFlt2(SHADER* sha, const char* uniform, const GLfloat* value)
{
	GET_AND_CHECK_UNIFORM;
	glUniform2fv(id, 1, value);
}

void tau_gra_shader_setuniformFlt3(SHADER* sha, const char* uniform, const GLfloat* value)
{
	GET_AND_CHECK_UNIFORM;
	glUniform3fv(id, 1, value);
}

// Destroy the given shader
void tau_gra_shader_destroy(SHADER* sha)
{
	glDeleteProgram(sha->program);
}

// Use the given sader
void tau_gra_shader_use(SHADER* sha)
{
	glUseProgram(sha->program);
}

#undef  TED_CURSUB
#define TED_CURSUB "setuniform"
int setuniform(GLint* outUniform, GLuint program, const char* name)
{
	*outUniform = glGetUniformLocation(program, name);
	if (*outUniform == -1)
	{
		return 1;
	}
	return 0;
}

#undef  TED_CURSUB
#define TED_CURSUB "initshader"
GLuint initshader(const std::string& shaderSource, GLenum shaderType)
{
	GLuint res;
	GLint err;
	GLchar* src = (GLchar*)malloc(sizeof(GLchar) * shaderSource.length() + 1);

	strcpy(src, shaderSource.c_str());
	src[shaderSource.length()] = '\0';

	// Use file's data as source for the shader
	res = glCreateShader(shaderType);
	glShaderSource(res, 1, &src, 0);
	glCompileShader(res);
	free(src);

	// Check for compilation error
	glGetShaderiv(res, GL_COMPILE_STATUS, &err);
	if (err == GL_FALSE)
	{
		// Buffer containing the error message
		GLint maxLength = 0;
		glGetShaderiv(res, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(res, maxLength, &maxLength, &errorLog[0]);

		// Print error
		std::string err;
		for (unsigned int i = 0; i < errorLog.size(); i++)
			err += errorLog.at(i);
		TED_PRINT_ERROR(err);

		glDeleteShader(res);
		throw CTauGraException("Shader compilation error");
	}
	return res;
}

