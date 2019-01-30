#pragma once
#include "../global.h"

#define TAU_TEXTUREUNIT_0 GL_TEXTURE0
#define TAU_TEXTUREUNIT_1 GL_TEXTURE1
typedef GLenum textureunit;
typedef struct _TEXTURE
{
	GLuint texture;
} TEXTURE;

// tau_gra_texture_make will create a TEXTURE using the data from a file
// tau_gra_texture_make_forframebuffer creates an empty texture with given W/H
TEXTURE tau_gra_texture_make(const char* filatpath);
TEXTURE tau_gra_texture_make_forframebuffer(int w, int h);

// Activate the given texture unit and use texture `t` for that specific unit.
// After returning, active texture unit is changed to the given value.
void    tau_gra_texture_use(TEXTURE* t, textureunit unit);

// Call this to make sure no texture is used.
// By default, this will use texture of ID 0.
//   glBindTexture(GL_TEXTURE_2D, 0);
void    tau_gra_texture_unuse(void);

void    tau_gra_texture_destroy(TEXTURE* t);

