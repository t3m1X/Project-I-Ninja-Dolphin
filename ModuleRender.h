#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"
#include "p2Point.h"
#include "pQueue.h"

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;

class ModuleRender : public Module
{
private:
	struct Blit_Request {
		SDL_Texture* text = nullptr;
		SDL_Rect* section = nullptr;
		SDL_Rect rect;
		double angle;

		Blit_Request(SDL_Texture* _text, SDL_Rect* _section, SDL_Rect _rect, double _angle) :
			text(_text), rect(_rect), angle(_angle)
		{
			if (_section != nullptr)
				section = new SDL_Rect(*_section);
		}
	};

	struct Quad_Request {
		Uint8 r, g, b, a;
		SDL_Rect rect;

		Quad_Request(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a, SDL_Rect _rect) :
			r(_r), g(_g), b(_b), a(_a), rect(_rect) 
		{}
	};

private:
	pQueue<Blit_Request> b_requests;
	pQueue<Quad_Request> quads;

public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PostUpdate();
	update_status PreUpdate();
	bool CleanUp();

	bool Blit(int layer, SDL_Texture* texture, int x, int y, iPoint direction = { 0,1 }, SDL_Rect* section = nullptr);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
};

#endif //__ModuleRenderer_H__