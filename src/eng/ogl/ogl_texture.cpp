#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <map>
#include "ogl_texture.h"
#include "ogl_exception.h"
#include "../print.h"

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_texture_make"
TEXTURE tau_gra_texture_make(const char* filepath)
{
	TEXTURE tex;
	SDL_Surface* surface = IMG_Load(filepath);
	if (!surface)
	{
		TED_PRINT_ERROR(std::string(IMG_GetError()));
		throw CTauGraException("Failed to load texture.");
	}

	GLint format = GL_RGBA;
	if (surface->format->Amask == 0)
		format = GL_RGB;

	glGenTextures(1, &tex.texture);
	glBindTexture(GL_TEXTURE_2D, tex.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(surface);

	return tex;
}

TEXTURE tau_gra_texture_make_forframebuffer(int w, int h)
{
	TEXTURE tex;
	
	glGenTextures(1, &tex.texture);
	glBindTexture(GL_TEXTURE_2D, tex.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

void tau_gra_texture_destroy(TEXTURE* tex)
{
	glDeleteTextures(1, &tex->texture);
}

void tau_gra_texture_use(TEXTURE* tex, textureunit unit)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, tex->texture);
}

void tau_gra_texture_unuse(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
