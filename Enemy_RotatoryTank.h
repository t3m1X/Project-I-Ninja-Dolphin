#ifndef __ENEMY_ROTATORYTANK_H__
#define __ENEMY_ROTATORYTANK_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"


enum ROUNDS
{
	NONE,
	FIRST,
	SECOND,
	THIRD,
	FOURTH
};

class Enemy_RotatoryTank : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation walk;
	Animation turret;
	Animation turret_shooting;
	Path path;
	iPoint original_position;
	int y_transition;
	bool has_transitioned = false;
	ROUNDS typology = ROUNDS::NONE;

public:

	Enemy_RotatoryTank(int x, int y);
	~Enemy_RotatoryTank();

	void Move();
	void Draw(SDL_Texture * sprites);
	void Shoot(iPoint origin, ROUNDS typology);
	void OnCollision(Collider* collider);
};


#endif
