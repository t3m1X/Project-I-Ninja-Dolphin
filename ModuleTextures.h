#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include "Module.h"
#include "Globals.h"

#define MAX_TEXTURES 50

struct SDL_Texture;
struct SDL_Surface;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const char* path);
	SDL_Texture* const SurfaceToTexture(SDL_Surface* surface);
	bool Unload(SDL_Texture* texture);

public:
	SDL_Texture* textures[MAX_TEXTURES];
	uint last_texture = 0;
};

#endif // __ModuleTextures_H__