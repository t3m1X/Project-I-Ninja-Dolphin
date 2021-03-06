#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"

#define NUM_MODULES 14

class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleRender;
class ModuleParticles;
class ModuleFonts;
class ModuleAudio;
class ModuleStage1;
class ModuleStageIntro;
class ModulePlayer;
class ModuleTransition;
class ModuleCollision;
class ModuleEnemies;
class ModuleBonus;
class Module;

class Application
{
public:

	Module* modules[NUM_MODULES];
	ModuleWindow* window;
	ModuleRender* render;
	ModuleInput* input;
	ModuleTextures* textures;
	ModuleFonts* fonts;
	ModuleAudio* audio;
	ModuleStageIntro* intro;
	ModuleStage1* stage1;
	ModuleCollision* collision;
	ModulePlayer* player;
	ModuleTransition* transition;
	ModuleParticles* particles;
	ModuleEnemies* enemies;
	ModuleBonus* bonus;	

	int game_loops = 1;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__