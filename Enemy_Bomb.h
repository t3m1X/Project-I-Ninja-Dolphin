#ifndef __ENEMY_PROYECTIL_H__
#define __ENEMY_PROYECTIL_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"


class Enemy_Proyectil : public Enemy
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

	Enemy_Proyectil(int x, int y);
	~Enemy_Proyectil();

	void Move();
	void Damaged();
	void OnCollision(Collider * collider);
};


#endif
