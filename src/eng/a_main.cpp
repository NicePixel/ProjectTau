#include <SDL2/SDL_mixer.h>
#include "print.h"
#include "a_main.h"

static Mix_Chunk* test_chunk = nullptr;

#undef  TED_CURSUB
#define TED_CURSUB "tau_aud_init"
void tau_aud_init(void)
{
	TED_PRINT_INFO("Starting the audio layer...");
	
	int flags = MIX_INIT_OGG|MIX_INIT_MOD;
	int initted=Mix_Init(flags);
	if((initted&flags) != flags)
	{
		std::string err = "Failed to initialize SDL2 Mixer";
		err += Mix_GetError();
		TED_PRINT_ERROR(err);
		throw CTauAudException(Mix_GetError());
	}
	
	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0)
	{
		TED_PRINT_ERROR("Failed to open audio");
		throw CTauAudException(Mix_GetError());
	}
	
	if (Mix_AllocateChannels(4) < 0)
	{
		TED_PRINT_ERROR("Failed to allocate mixing channels");
		throw CTauAudException(Mix_GetError());
	}
	
	
	test_chunk = Mix_LoadWAV("data/audio/joker.wav");
	
	tau_aud_test(90, 1000);
		
}

#undef  TED_CURSUB
#define TED_CURSUB "tau_aud_test"
void tau_aud_test(int angle, int dist)
{
	if (!Mix_SetPosition(2, angle, dist))
	{
		TED_PRINT_ERROR("Positioning fialed");
		TED_PRINT_ERROR(std::string(Mix_GetError()));
	}
}

#undef  TED_CURSUB
#define TED_CURSUB "tau_aud_quit"
void tau_aud_quit(void)
{
	Mix_Quit();
}

void tau_aud_test(int run)
{
	if (run != 1)
		return;
	
	Mix_PlayChannel(2, test_chunk, 0);
}

void tau_aud_vol(int vol)
{
	Mix_Volume(2, vol);
}