#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_mixer/include/SDL_mixer.h"

#define MAX_SFX 16
#define MAX_MUSIC 10
#define CHUNKSIZE 1024

class ModuleAudio : public Module 
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	Mix_Chunk* const LoadSFX(const char* path);
	void FreeSFX(Mix_Chunk* sfx);
	Mix_Music* const LoadMusic(const char* path);
	void FreeMusic(Mix_Music* music);

	void const PlaySFX(Mix_Chunk* sfx);
	void const PlayMusic(Mix_Music* music);
	void const MusicVolume(int percent = 100);
	void const StopMusic();

public:
	Mix_Chunk* sfxs[MAX_SFX];
	uint last_sfx = 0;

	Mix_Music* musics[MAX_MUSIC];
	uint last_music = 0;
};

#endif