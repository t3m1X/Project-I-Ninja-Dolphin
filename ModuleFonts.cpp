#include "ModuleFonts.h"

ModuleFonts::ModuleFonts()
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
}

bool ModuleFonts::CleanUp()
{
	return false;
}

TTF_Font * const ModuleFonts::LoadFont(const char * path, uint ptsize)
{
	return nullptr;
}

void const ModuleFonts::EraseFont(TTF_Font * font)
{
	return nullptr;
}

void const ModuleFonts::WriteText(TTF_Font * font, const char * text, SDL_Color color)
{
	return nullptr;
}
