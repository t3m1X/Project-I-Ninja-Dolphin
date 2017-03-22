#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "Globals.h"

#define MAX_SFX 50
#define MAX_MUSIC 10
#define CHUNKSIZE 1024

struct Mix_Chunk;
struct Mix_Music;

class ModuleAudio : public Module 
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	Mix_Chunk* const LoadSFX(const char* path);
	Mix_Music* const LoadMusic(const char* path);

	void const PlaySFX(Mix_Chunk* sfx);
	void const PlayMusic(Mix_Music* music);
	void const StopMusic();

public:
	Mix_Chunk* sfx[MAX_SFX];
	uint last_sfx = 0;

	Mix_Music* musics[MAX_MUSIC];
	uint last_music = 0;
};

#endif