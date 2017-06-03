#ifndef __ModuleBonus_H__
#define __ModuleBonus_H__

#include "Module.h"
#include "Animation.h"

#define MAX_BONUS 100
#define SPAWN_MARGIN 200

enum BONUS_TYPE
{
	NO_BONUS_TYPE,
	RED_BONUS,
	BLUE_BONUS,
	MISSILE_BONUS,
	BOMB_BONUS,
	MEDAL_BONUS
};

class Bonus {
public:
	Collider* col;
	iPoint position;
	BONUS_TYPE type = BONUS_TYPE::NO_BONUS_TYPE;

public:
	Bonus(int x, int y, BONUS_TYPE _type) : position(iPoint(x, y)), type(_type)
	{}
	~Bonus();

	virtual void Update() {}
	
};

class PowerUp : public Bonus {
private:
	iPoint bonus_position;
	int circle_iterations = 0;
	uint sdl_clock_next;
	iPoint new_pos = { 0,0 };

public:
	SDL_Rect shadow;

public:
	PowerUp(int x, int y, BONUS_TYPE _type) : Bonus(x, y, _type) , bonus_position(iPoint(x,y))
	{
		sdl_clock_next = SDL_GetTicks() + 3000;
	}

	~PowerUp()
	{}

	void Update();
};

class ModuleBonus : public Module
{
public:
	ModuleBonus();
	~ModuleBonus();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool AddBonus(BONUS_TYPE type, int x, int y);

private:

	Bonus* bonus[MAX_BONUS];
	SDL_Texture* sprites;

public:
	//Animations
	Animation blue_bonus;
	Animation red_bonus;
	Animation missile_bonus;
	Animation bomb_bonus;
	Animation medal_bonus;
	Animation medal_bonus_dissapearing;

};

#endif
