/* ogl_main.h
 * Initialization functions and small utilities.
 */
#pragma once
#include "../global.h"

// Initialize the graphics subsystem
// Calls SDL init functions, GLEW, create a GL context, ...
// Exceptions:
// 	CGraException
void tau_gra_init(const char* windowTitle);

// Show the current render to the screen
void tau_gra_updatewindow(void);

// Clear the current framebuffer's buffer
// Change the current framebuffer with `tau_gra_framebuffer_use`,
// check "ogl_framebuffer.h" for info.
//
// By default this clears the screen.
#define TAU_CLEAR_COLORBUFFER 1
#define TAU_CLEAR_DEPTHBUFFER 2
#define TAU_CLEAR_COLORANDDEPTHBUFFER 3
void tau_gra_clear(int what);

void tau_gra_disableDepthTest(void);
void tau_gra_enableDepthTest(void);
