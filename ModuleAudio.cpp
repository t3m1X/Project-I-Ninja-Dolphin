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

	Mix_AllocateChannels(MAX_SFX);

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

	int current_sfx = 0;
	for (; current_sfx < MAX_SFX; ++current_sfx)
	{
		if (sfxs[current_sfx] == nullptr)
			break;
	}
	if (current_sfx == MAX_SFX) {
		LOG("Overflow error: Overwriting sfx");
		Mix_FreeChunk(sfxs[last_sfx]);
		current_sfx = last_sfx++;
		if (last_sfx == MAX_SFX)
			last_sfx = 0;
	}

	sfxs[current_sfx] = Mix_LoadWAV(path);

	if (sfxs[last_sfx] == NULL) {
		LOG("MixLoadWav: Failed to load wav from path \"%s\": %s\n", path, Mix_GetError());
	}
	else
		ret = sfxs[last_sfx];

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

	int current_music = 0;
	for (; current_music < MAX_SFX; ++current_music)
	{
		if (musics[current_music] == nullptr)
			break;
	}

	if (current_music == MAX_MUSIC) {
		LOG("Overflow error: Overwriting music");
		Mix_FreeMusic(musics[last_music]);
		current_music = last_music++;
		if (last_music == MAX_MUSIC)
			last_music = 0;
	}

	musics[current_music] = Mix_LoadMUS(path);

	if (!musics[last_music]) {
		LOG("Mix_LoadMUS: Could not load \"%s\": %s\n", path, Mix_GetError());
	}
	else 
		ret = musics[last_music];

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
	uint i = 0;
	for (; i < MAX_SFX; ++i)
		if (sfx == sfxs[i]) break;
		
	Mix_HaltChannel(i);
	if (Mix_PlayChannel(i, sfx, 0) == -1) 
		LOG("Mix_PlayChannel: Could not play sfx: %s\n", Mix_GetError());
}

void const ModuleAudio::PlayMusic(Mix_Music * music) 
{
	if (music == playing)
		return;
	//This will halt previous music if StopMusic() was not called, will fade-in in 5 seconds
	//Music will play on infinite-loop
	if (Mix_FadeInMusic(music, -1, 5000) == -1) {
		LOG("Mix_FadeInMusic: Could not play music, error: %s\n", Mix_GetError());
	}
	else 
		playing = music;

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
	playing = nullptr;
}
