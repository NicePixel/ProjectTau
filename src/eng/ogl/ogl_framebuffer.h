#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include "ogl_texture.h"

typedef struct _FRAMEBUFFER
{
	GLuint framebuffer;
	TEXTURE attachedtexture;
} FRAMEBUFFER;

FRAMEBUFFER tau_gra_framebuffer_make(int w, int h);
void tau_gra_framebuffer_destroy(FRAMEBUFFER* fb);
void tau_gra_framebuffer_use(FRAMEBUFFER* fb);
