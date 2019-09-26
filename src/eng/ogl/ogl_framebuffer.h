/* ogl_framebuffer.h
 * Framebuffer structure type.
 */
#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include "ogl_texture.h"

// Every framebuffer can have a texture attached to it.
// This is the only behaviour this implementation has.
//
// If the created framebuffer is used (call `tau_gra_framebuffer_use`),
// then all the subsequent rendering (drawing) calls will render to the 
// attached texture of the used framebuffer
//
// Use tau_gra_framebuffer_use(0) to use the default framebuffer, which is the 
// direct screen framebuffer.
// Use this for rendering to the screen.
typedef struct _FRAMEBUFFER
{
	GLuint framebuffer;
	TEXTURE attachedtexture;
} FRAMEBUFFER;

FRAMEBUFFER tau_gra_framebuffer_make(int w, int h);
void tau_gra_framebuffer_destroy(FRAMEBUFFER* fb);
void tau_gra_framebuffer_use(FRAMEBUFFER* fb);
