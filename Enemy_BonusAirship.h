#ifndef __ENEMY_BONUSAIRSHIP_H__
#define __ENEMY_BONUSAIRSHIP_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"

class Enemy_BonusAirship : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation fly;
	Path path;
	iPoint original_position;

public:

	Enemy_BonusAirship(int x, int y);
	~Enemy_BonusAirship();

	void Move();
};

#endif