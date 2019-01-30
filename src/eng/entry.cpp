#include "print.h"
#ifdef TED_OGL
#include "ogl/ogl_main.h"
#include "ogl/ogl_exception.h"
#include "ogl/ogl_test.h"
#include "ogl/ogl_font.h"
#include "ogl/ogl_mesh.h"
#else
#error "Vulkan is not implement"
#endif

#undef  TED_CURSUB
#define TED_CURSUB "tau_init"
void tau_init(const char* programName)
{
	int allsystemsgo = 1;

	TED_PRINT_INFO("Initializing program");
	try
	{
		tau_gra_init(programName);
		tau_gra_font_init();
		tau_gra_mesh_init();
	}
	catch (CTauGraException e)
	{
		TED_PRINT_ERROR("Graphics initialization error! Error message is in the next printed line.");
		TED_PRINT_ERROR(e.what());
		allsystemsgo = 0;
	}

	if (!allsystemsgo)
	{
		TED_PRINT_ERROR("The program failed to initialize one or more subsystems.");
		TED_PRINT_ERROR("We're done here, unfortunately.");
		return;
	}

	TED_PRINT_INFO("Initialization seems successful.");
	if(0)
		tau_gra_test_runall();
}

#undef  TED_CURSUB
#define TED_CURSUB "tau_quit"
void tau_quit(void)
{
	TED_PRINT_INFO("semi-STUB");
	tau_gra_font_quit();
	tau_gra_mesh_quit();
}
