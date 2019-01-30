#include "ogl_main.h"
#include "ogl_exception.h"
#include "ogl_shader.h"
#include "ogl_mesh.h"
#include "ogl_texture.h"
#include "../print.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

static SDL_Window* window = nullptr;
static SDL_GLContext context;

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_init"
void tau_gra_init(const char* windowTitle)
{
	GLenum e;

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw CTauGraException(SDL_GetError());

	// Init window
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	if (!window)
		throw CTauGraException(SDL_GetError());
	context = SDL_GL_CreateContext(window);

	// Init GLEW
	glewExperimental = true;
	e = glewInit();
	if (e != GLEW_OK)
		throw CTauGraException(reinterpret_cast<const char*>(glewGetErrorString(e)));

	// Initial state
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
}

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_updatewindow"
void tau_gra_updatewindow(void)
{
	SDL_GL_SwapWindow(window);
}

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_updatewindow"
void tau_gra_clear(int what)
{
	if (what == TAU_CLEAR_COLORBUFFER)
		glClear(GL_COLOR_BUFFER_BIT);
	if (what == TAU_CLEAR_DEPTHBUFFER)
		glClear(GL_DEPTH_BUFFER_BIT);
	if (what == TAU_CLEAR_COLORANDDEPTHBUFFER)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void tau_gra_disableDepthTest(void)
{
	glDisable(GL_DEPTH_TEST);
}
void tau_gra_enableDepthTest(void)
{
	glEnable(GL_DEPTH_TEST);
}

/* Courtesy of http://ascii-art.de/
 * Author: SSt


                                                       ____________
                                 (`-..________....---''  ____..._.-`
                                  \\`._______.._,.---'''     ,'
                                  ; )`.      __..-'`-.      /
                                 / /     _.-' _,.;;._ `-._,'
                                / /   ,-' _.-'  //   ``--._``._
                              ,','_.-' ,-' _.- (( =-    -. `-._`-._____
                            ,;.''__..-'   _..--.\\.--'````--.._``-.`-._`.
             _          |\,' .-''        ```-'`---'`-...__,._  ``-.`-.`-.`.
  _     _.-,'(__)\__)\-'' `     ___  .          `     \      `--._
,',)---' /|)          `     `      ``-.   `     /     /     `     `-.
\_____--.  '`  `               __..-.  \     . (   < _...-----..._   `.
 \_,--..__. \\ .-`.\----'';``,..-.__ \  \      ,`_. `.,-'`--'`---''`.  )
           `.\`.\  `_.-..' ,'   _,-..'  /..,-''(, ,' ; ( _______`___..'__
                   ((,(,__(    ((,(,__,'  ``'-- `'`.(\  `.,..______      
                                                      ``--------..._``--.__
*/
