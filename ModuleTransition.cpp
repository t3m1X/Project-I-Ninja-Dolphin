#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleTransition.h"
#include "ModuleRender.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "ModuleStage2.h"
#include "ModuleStageIntro.h"
#include "SDL\include\SDL_rect.h"
#include "ModuleTextures.h"

ModuleTransition::ModuleTransition()
{
	screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

ModuleTransition::~ModuleTransition()
{}

// Load assets
bool ModuleTransition::Start()
{
	LOG("Preparing transition Screen");
	loading_screen = App->textures->Load("revamp_spritesheets/LoadingScreenAnimation.png");
	loading_screen_animation.SetUp(0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 10, 10, "9,8,7,6,5,4,3,2,1,0");
	loading_screen_animation.loop = false;
	loading_screen_animation.speed = 0.3f;
	loading_screen_animation_b.SetUp(0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 10, 10, "0,1,2,3,4,5,6,7,8,9");
	loading_screen_animation_b.loop = false;
	loading_screen_animation_b.speed = 0.5f;
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
update_status ModuleTransition::Update()
{
	if (current_step == transition_step::none)
		return UPDATE_CONTINUE;

	Uint32 now = SDL_GetTicks() - start_time;

	switch (current_step)
	{
	case transition_step::transition_to_black:
	{
		if (now >= total_time && loading_screen_animation.Finished())
		{
			// ---
			to_disable->Disable();
			to_enable->Enable();
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = transition_step::transition_from_black;
		}
		App->render->Blit(loading_screen, 0, 0, &loading_screen_animation.GetCurrentFrame());
	} break;

	case transition_step::transition_from_black:
	{
		if (now >= total_time && loading_screen_animation_b.Finished())
			current_step = transition_step::none;
		App->render->Blit(loading_screen, 0, 0, &loading_screen_animation_b.GetCurrentFrame());
	} break;
	}

	return UPDATE_CONTINUE;
}

bool ModuleTransition::CleanUp() {
	bool ret = true;
	loading_screen_animation.CleanUp();
	loading_screen_animation_b.CleanUp();

	return ret;
}

// transition to black. At mid point deactivate one module, then activate the other
bool ModuleTransition::Transition(Module* module_off, Module* module_on, float time)
{
	bool ret = false;

	if (current_step == transition_step::none)
	{
		loading_screen_animation.Reset();
		loading_screen_animation_b.Reset();
		loading_screen_animation.speed = 10 / 60.0f / time;
		loading_screen_animation_b.speed = 10 / 60.0f / time;

		current_step = transition_step::transition_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		to_disable = module_off;
		to_enable = module_on;
		ret = true;
	}

	return ret;
}