#ifndef __ModuleBonus_H__
#define __ModuleBonus_H__

#include "Module.h"

#define MAX_BONUS 100
#define SPAWN_MARGIN 200

enum BONUS_TYPE
{
	NO_BONUS_TYPE,
	RED
};

class Bonus {
	iPoint position;
	BONUS_TYPE type = BONUS_TYPE::NO_BONUS_TYPE;

	Bonus(iPoint _position, BONUS_TYPE _type) : position(_position), type(_type) 
	{}

	virtual void Update() {}
	void OnColl(Collider* player);
};

class PowerUp : public Bonus {
	iPoint bonus_position;
	int circle_iterations = 0;
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
};

#endif
