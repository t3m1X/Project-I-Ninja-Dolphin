#include "Application.h"
#include "Globals.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

ModuleFonts::ModuleFonts() : Module()
{
}

ModuleFonts::~ModuleFonts()
{
}

bool ModuleFonts::Init()
{
	bool ret = true;
	for (uint i = 0; i < MAX_FONTS; ++i)
		fonts[i] = nullptr;

	for (uint i = 0; i < MAX_TEXTS; ++i)
		texts[i] = nullptr;

	if (TTF_Init() == -1) {
		LOG("TTF_Init: %s\n", TTF_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleFonts::CleanUp()
{
	bool ret = true;
	for (int i = MAX_FONTS - 1; i >= 0; --i) {
		TTF_CloseFont(fonts[i]);
		fonts[i] = nullptr;
	}

	TTF_Quit();
	return ret;
}

update_status ModuleFonts::PreUpdate()
{
	//Erasing textures used in last tick
	for (uint i = 0; i < MAX_TEXTS; ++i) {
		if (texts[i] != nullptr) {
			SDL_DestroyTexture(texts[i]);
			texts[i] = nullptr;
		}
	}

	return update_status::UPDATE_CONTINUE;
}

TTF_Font * const ModuleFonts::LoadFont(const char * path, int ptsize)
{
	TTF_Font* ret = nullptr;
	if (last_font == MAX_FONTS) {
		LOG("Overflow error: Overwriting fonts");
		last_font = 0;
	}

	fonts[last_font] = TTF_OpenFont(path, ptsize);

	if (fonts[last_font] == NULL) {
		LOG("TTF_OpenFont: %s\n", TTF_GetError());
	}
	else
		ret = fonts[last_font++];

	return ret;
}

void const ModuleFonts::EraseFont(TTF_Font * font)
{
	if (font == nullptr)
		return;

	for (uint i = 0; i < MAX_FONTS; ++i) {
		if (fonts[i] == font)
			TTF_CloseFont(fonts[i]);
			fonts[i] = nullptr;
	}
	
}

void const ModuleFonts::WriteText(TTF_Font * font, const char * text, int x, int y, SDL_Color color)
{
	SDL_Surface* text_surface;
	if (!(text_surface = TTF_RenderText_Blended(font, text, color))) {
		LOG("TTF_RenderText: %s\n", TTF_GetError());
	}
	else {
		for (uint i = 0; i < MAX_TEXTS; ++i) {
			if (texts[i] == nullptr) {
				texts[i] = App->textures->SurfaceToTexture(text_surface);
				if (texts[i] == NULL)
					LOG("%s", SDL_GetError());
				App->render->Blit(texts[i], x, y);
				break;
			}
		}
		SDL_FreeSurface(text_surface);
	}
}
