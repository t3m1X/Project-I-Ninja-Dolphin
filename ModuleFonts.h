#ifndef __ModuleFonts_H__
#define __Modulefonts_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_ttf\include\SDL_ttf.h"

#define MAX_FONTS 25

class ModuleFonts : public Module {
public:
	ModuleFonts();
	~ModuleFonts();

	bool Init();
	bool CleanUp();

	TTF_Font* const LoadFont(const char* path, uint ptsize);
	void const EraseFont(TTF_Font* font);
	void const WriteText(TTF_Font* font, const char* text, SDL_Color color);

public:
	TTF_Font* fonts[MAX_FONTS];
	uint last_font = 0;
};

#endif //__ModuleFonts_H__