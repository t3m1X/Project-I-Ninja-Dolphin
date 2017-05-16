#ifndef __ENEMY_TURRET_H__
#define __ENEMY_TURRET_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"

class Enemy_Turret : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation walk;
	Path path;
	iPoint original_position;
	int shots = 0;

public:

	Enemy_Turret(int x, int y);
	~Enemy_Turret();

	void Move();
	void OnCollision(Collider * collider);
};



#endif
