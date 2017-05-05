#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"



ModuleRender::ModuleRender() : Module()
{}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(REN_VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == NULL)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleRender::PreUpdate()
{
	SDL_RenderClear(renderer);

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	while (b_requests.Size() != 0) {
		SDL_Rect temp_rect = b_requests[0].rect;
		if (SDL_RenderCopyEx(renderer, b_requests[0].text, b_requests[0].section, &temp_rect, b_requests[0].angle, NULL, SDL_FLIP_NONE) != 0)
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		if (b_requests[0].section != nullptr)
			delete[] b_requests[0].section;
		b_requests.pop_front();
	}

	while (quads.Size() != 0) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, quads[0].r, quads[0].g, quads[0].b, quads[0].a);

		SDL_Rect tmp = quads[0].rect;
		if (SDL_RenderFillRect(renderer, &tmp) != 0) {
			LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		}
		quads.pop_front();
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
	SDL_RenderPresent(renderer);
	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	while (b_requests.Size() != 0)
	{
		if (b_requests[0].section != nullptr)
			delete[] b_requests[0].section;

		b_requests.pop_front();
	}

	while (quads.Size() != 0)
		quads.pop_front();

	//Destroy window
	if(renderer != nullptr)
		SDL_DestroyRenderer(renderer);

	return true;
}

// Blit to screen
bool ModuleRender::Blit(int layer, SDL_Texture* texture, int x, int y, iPoint direction, SDL_Rect* section)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int)(-camera.x + x) * SCREEN_SIZE;
	rect.y = (int)(-camera.y + y) * SCREEN_SIZE;

	if(section != nullptr)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	iPoint down = { 0,1 };
  	double angle = down.Angle(direction);
	if (direction.x > 0)
		angle = -angle;

	b_requests.push_back(Blit_Request(texture,section,rect,angle), layer);

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(-camera.x + rect.x) * SCREEN_SIZE;
		rec.y = (int)(-camera.y + rect.y) * SCREEN_SIZE;
		rec.w *= SCREEN_SIZE;
		rec.h *= SCREEN_SIZE;
	}


	quads.push_back(Quad_Request(r,g,b,a,rec),0);

	return ret;
}
