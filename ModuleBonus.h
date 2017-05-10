#ifndef __ModuleBonus_H__
#define __ModuleBonus_H__

#include "Module.h"

#define MAX_BONUS 100
#define SPAWN_MARGIN 200

enum BONUS_TYPE
{
	NO_BONUS_TYPE,
	RED_BONUS,
	MEDAL
};

class Bonus {
protected:
	iPoint position;
	Animation animation;
	BONUS_TYPE type = BONUS_TYPE::NO_BONUS_TYPE;

public:
	Bonus(int x, int y, BONUS_TYPE _type) : position(iPoint(x,y)), type(_type) 
	{}

	virtual void Update() {}
	virtual void OnColl(Collider* player);
	virtual iPoint getPosition() {
		return position;
	}
	virtual void Draw(SDL_Texture* sprite);
};

class PowerUp : public Bonus {
private:
	iPoint bonus_position;
	int circle_iterations = 0;

public:
	PowerUp(int x, int y, BONUS_TYPE _type) : Bonus(x, y, _type) 
	{}

	iPoint getPosition() {
		return bonus_position;
	}

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
};

#endif
