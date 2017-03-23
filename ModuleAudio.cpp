#include "ModuleAudio.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio() 
{
	for (uint i = 0; i < MAX_SFX; ++i)
		sfx[i] = nullptr;

	for (uint i = 0; i < MAX_MUSIC; ++i)
		musics[i] = nullptr;
}

ModuleAudio::~ModuleAudio() 
{}

bool ModuleAudio::Init() {
	bool ret = true;
	int flags = MIX_INIT_OGG;
	int initted = Mix_Init(flags);
	if (initted&flags != flags) {
		LOG("Mix_Init: Failed to init required ogg support! %s\n", Mix_GetError());
		ret = false;
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, CHUNKSIZE) == -1) { //2 is for stereo sound
		LOG("Mix_OpenAudio: Failed to open audio! %s\n", Mix_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleAudio::CleanUp()
{
	LOG("Freeing sound effects and music");

	Mix_HaltChannel(-1); //Stop playing all sfx to avoid problems.
	for (int i = MAX_SFX - 1; i >= 0; --i) {
		if (sfx[i] != nullptr)
			Mix_FreeChunk(sfx[i]);
		sfx[i] = nullptr;
	}

	if (Mix_PlayingMusic())
		Mix_HaltMusic(); //Stop playing music to avoid problems.
	for (int i = MAX_MUSIC - 1; i >= 0; --i) {
		if (musics[i] != nullptr)
			Mix_FreeMusic(musics[i]);
		musics[i] = nullptr;
	}

	Mix_CloseAudio();

	return true;
}

Mix_Chunk * const ModuleAudio::LoadSFX(const char * path) 
{
	Mix_Chunk* ret = nullptr;

	sfx[last_sfx] = Mix_LoadWAV(path);

	if (sfx[last_sfx] == NULL) {
		LOG("MixLoadWav: Failed to load wav from path \"%s\": %s\n", path, Mix_GetError());
	}
	else
		ret = sfx[last_sfx++];

	return ret;
}

Mix_Music * const ModuleAudio::LoadMusic(const char * path) 
{
	Mix_Music* ret = nullptr;

	musics[last_music] = Mix_LoadMUS(path);

	if (!musics[last_music]) {
		LOG("Mix_LoadMUS: Could not load \"%s\": %s\n", path, Mix_GetError());
	}
	else 
		ret = musics[last_music++];

	return ret;
}

void const ModuleAudio::PlaySFX(Mix_Chunk * sfx) 
{
	if (Mix_PlayChannel(1, sfx, 0) == -1) 
		LOG("Mix_PlayChannel: Could not play sfx: %s\n", Mix_GetError());
}

void const ModuleAudio::PlayMusic(Mix_Music * music) 
{
	//This will halt previous music if StopMusic() was not called, will fade-in in 5 seconds
	//Music will play on infinite-loop
	if (Mix_FadeInMusic(music, -1, 5000) == -1)
		LOG("Mix_FadeInMusic: Could not play music, error: %s\n", Mix_GetError());

}

void const ModuleAudio::StopMusic() 
{
	//Fades-out 5 seconds
	Mix_FadeOutMusic(5000);
}
