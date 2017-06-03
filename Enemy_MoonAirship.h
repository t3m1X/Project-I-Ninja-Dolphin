#ifndef __ENEMY_MOONAIRSHIP_H__
#define __ENEMY_MOONAIRSHIP_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"

class Enemy_MoonAirship : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation fly;
	Path path;
	iPoint original_position;
	int shots = 0;

public:

	Enemy_MoonAirship(int x, int y);
	~Enemy_MoonAirship();

	void Move();
	/*void Shoot(iPoint origin);*/
	void OnCollision(Collider* collider);
};

#endif
