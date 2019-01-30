#pragma once

#include "object.h"

// Include these because they may be useful for the unit that wants to render.
#ifdef TED_OGL
#include "../eng/ogl/ogl_render.h"
#include "../eng/ogl/ogl_shader.h"
#include "../eng/ogl/ogl_mesh.h"
#include "../eng/ogl/ogl_main.h"
#include "../eng/ogl/ogl_texture.h"
#else
#error "Vulkan is not implement"
#endif

void tau_gra_ren_object(CTauObject* obj);
