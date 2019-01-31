/* render.h
 * Render functions independent of the currently used API
 */
#pragma once

// Include functions that work differently depending on the graphics API used
#include "r_main.h"
#include "r_texture.h"
#include "r_shader.h"
#include "r_mesh.h"
#include "r_font.h"
#include "r_framebuffer.h"
#ifdef TED_OGL
#include "ogl/ogl_render.h"
#else
#error "Vulkan is not implemented"
#endif

#include "object.h"

void tau_gra_ren_object(CTauObject* obj);
