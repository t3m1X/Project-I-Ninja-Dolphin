#include "ModuleAudio.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio() 
{
	for (uint i = 0; i < MAX_SFX; ++i)
		sfxs[i] = nullptr;

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
		if (sfxs[i] != nullptr)
			Mix_FreeChunk(sfxs[i]);
		sfxs[i] = nullptr;
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

	if (last_sfx == MAX_MUSIC) {
		LOG("Overflow error: Overwriting sfx");
		last_sfx = 0;
	}

	sfxs[last_sfx] = Mix_LoadWAV(path);

	if (sfxs[last_sfx] == NULL) {
		LOG("MixLoadWav: Failed to load wav from path \"%s\": %s\n", path, Mix_GetError());
	}
	else
		ret = sfxs[last_sfx++];

	return ret;
}

void ModuleAudio::FreeSFX(Mix_Chunk * sfx)
{
	if (sfx == nullptr)
		return;

	for (int i = 0; i < MAX_SFX; ++i) {
		if (sfxs[i] == sfx) {
			Mix_FreeChunk(sfxs[i]);
			sfxs[i] = nullptr;
		}
	}


}

Mix_Music * const ModuleAudio::LoadMusic(const char * path) 
{
	Mix_Music* ret = nullptr;

	if (last_music == MAX_MUSIC) {
		LOG("Overflow error: Overwriting music");
		last_music = 0;
	}

	musics[last_music] = Mix_LoadMUS(path);

	if (!musics[last_music]) {
		LOG("Mix_LoadMUS: Could not load \"%s\": %s\n", path, Mix_GetError());
	}
	else 
		ret = musics[last_music++];

	return ret;
}

void ModuleAudio::FreeMusic(Mix_Music * music)
{
	if (music = nullptr)
		return;
	for (int i = 0; i < MAX_MUSIC; ++i) {
		if (musics[i] == music) {
			Mix_FreeMusic(musics[i]);
			musics[i] = nullptr;
		}
	}
	

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

void const ModuleAudio::MusicVolume(int percent) {
	if (percent > 100)
		percent = 100;
	LOG("Volume was: %d\n", Mix_VolumeMusic(MIX_MAX_VOLUME * percent / 100.0f));
	LOG("Volume is now: %d\n", Mix_VolumeMusic(-1));
	return;
}

void const ModuleAudio::StopMusic() 
{
	//Fades-out 2 seconds
	Mix_FadeOutMusic(2000);
}
