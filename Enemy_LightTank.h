#ifndef __ENEMY_LIGHTTANK_H__
#define __ENEMY_LIGHTTANK_H__


#include "Enemy.h"
#include "Path.h"
#include "Globals.h"


class Enemy_LightTank : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation walk;
	Path path;
	iPoint original_position;

public:

	Enemy_LightTank(int x, int y);
	~Enemy_LightTank();

	void Move();
};




#endif