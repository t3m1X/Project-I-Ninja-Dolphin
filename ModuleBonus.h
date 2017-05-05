#ifndef __ModuleBonus_H__
#define __ModuleBonus_H__

#include "Module.h"

#define MAX_BONUS 100

enum BONUS_TYPE
{
	NO_BONUS_TYPE,
	RED
};

class Bonus;

struct BonusInfo
{
	BONUS_TYPE type = BONUS_TYPE::NO_BONUS_TYPE;
	int x, y;
};

class ModuleBonus : public Module
{
public:

	ModuleBonus();
	~ModuleBonus();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool AddBonus(BONUS_TYPE type, int x, int y);

private:

	void SpawnBonus(const BonusInfo& info);

private:

	BonusInfo queue[MAX_BONUS];
	Bonus* bonus[MAX_BONUS];
	SDL_Texture* sprites;
};

#endif
