#ifndef __ENEMY_BOX_H__
#define __ENEMY_BOX_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"

class Enemy_Box : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation walk;
	Path path;
	iPoint original_position;

public:

	Enemy_Box(int x, int y);
	~Enemy_Box();

	void Move();
	void OnCollision(Collider * collider);
};



#endif
