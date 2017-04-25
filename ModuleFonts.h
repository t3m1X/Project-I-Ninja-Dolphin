#ifndef __ModuleFonts_H__
#define __ModuleFonts_H__

#include "Module.h"
#include "SDL_ttf\include\SDL_ttf.h"

#define MAX_FONTS 25
#define MAX_TEXTS 50

class ModuleFonts : public Module {
public:
	ModuleFonts();
	~ModuleFonts();

	bool Init();
	bool CleanUp();

	update_status PreUpdate();

	TTF_Font* const LoadFont(const char* path, int ptsize);
	void const EraseFont(TTF_Font* font);
	void const WriteText(TTF_Font* font, const char* text, int x, int y, SDL_Color color);

public:
	TTF_Font* fonts[MAX_FONTS];
	uint last_font = 0;

	SDL_Texture* texts[MAX_TEXTS];
};

#endif //__ModuleFonts_H__