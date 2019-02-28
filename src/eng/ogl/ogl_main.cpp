#include "ogl_main.h"
#include "ogl_exception.h"
#include "ogl_shader.h"
#include "ogl_mesh.h"
#include "ogl_font.h"
#include "ogl_texture.h"
#include "../print.h"
#include "../configuration.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

static SDL_Window* window = nullptr;
static SDL_GLContext context;
static TVIDEO_INFO videoinfo;

#undef  TED_CURSUB
#define TED_CURSUB "tau_gra_init"
void tau_gra_init(const char* windowTitle)
{
	nlohmann::json config = tau_config_load("data/configurations/graphical.json");
	GLenum e;
	
	// No error checking, nor correction.
	// Rationale behind this is that if the configuration file is bad,
	// then the user has altered it, but not correctly. This should be told to
	// the user.
	videoinfo.width   = config["videwidth"];
	videoinfo.height  = config["videoheight"];
	videoinfo.mode    = config["videomode"];
	videoinfo.clear_r = config["videodefaultcolour"][0];
	videoinfo.clear_g = config["videodefaultcolour"][1];
	videoinfo.clear_b = config["videodefaultcolour"][2];

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw CTauGraException(SDL_GetError());

	// Init window
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, videoinfo.width, videoinfo.height, SDL_WINDOW_OPENGL | videoinfo.mode);
	if (!window)
		throw CTauGraException(SDL_GetError());
	context = SDL_GL_CreateContext(window);

	// Init GLEW
	glewExperimental = true;
	e = glewInit();
	if (e != GLEW_OK)
		throw CTauGraException(reinterpret_cast<const char*>(glewGetErrorString(e)));

	// Initial state
	glClearColor(videoinfo.clear_r, videoinfo.clear_g, videoinfo.clear_b, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
	tau_gra_enableDepthTest();
	
	// Call other initialization functions
	tau_gra_font_init();
	tau_gra_mesh_init();
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
	glClear(what);
}

void tau_gra_disableDepthTest(void)
{
	glDisable(GL_DEPTH_TEST);
}
void tau_gra_enableDepthTest(void)
{
	glEnable(GL_DEPTH_TEST);
}

TVIDEO_INFO tau_gra_videogetinfo(void)
{
	return videoinfo;
}

void tau_gra_videomode(int mode)
{
	SDL_SetWindowFullscreen(window, mode);
	videoinfo.mode = mode;
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
