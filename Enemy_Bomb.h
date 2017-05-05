#ifndef __ENEMY_BOMB_H__
#define __ENEMY_BOMB_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"


class Enemy_Bomb : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation walk;
	Animation damage;
	Path path;
	iPoint original_position;

public:

	Enemy_Bomb(int x, int y);
	~Enemy_Bomb();

	void Move();
	void Damaged();
	void OnCollision(Collider * collider);
};


#endif
