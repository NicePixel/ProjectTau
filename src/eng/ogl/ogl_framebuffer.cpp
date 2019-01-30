#include "ogl_framebuffer.h"
#include "ogl_exception.h"
#include "../print.h"

FRAMEBUFFER tau_gra_framebuffer_make(int w, int h)
{
	FRAMEBUFFER fb;

	GLenum code;

	glGenFramebuffers(1, &fb.framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fb.framebuffer);

	// Texture attachment
	fb.attachedtexture = tau_gra_texture_make_forframebuffer(w, h);
	glBindTexture(GL_TEXTURE_2D, fb.attachedtexture.texture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.attachedtexture.texture, 0);

	// Render buffer object for depth/stencil testing
	GLuint rboDepthStencil;
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		TED_PRINT_ERROR(std::string("Framebuffer isn't complete! Got code: ") + std::to_string(code));
		throw CTauGraException("Framebuffer isn't complete");
	}

	return fb;
}

void tau_gra_framebuffer_destroy(FRAMEBUFFER* fb)
{
	tau_gra_texture_destroy(&fb->attachedtexture);
	glDeleteFramebuffers(1, &fb->framebuffer);
}

void tau_gra_framebuffer_use(FRAMEBUFFER* fb)
{
	glDepthMask(GL_TRUE);
	GLuint id = 0;
	if (fb)
		id = fb->framebuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}
