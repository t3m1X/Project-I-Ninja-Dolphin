#ifndef __ENEMY_KAMIKAZE_H__
#define __ENEMY_KAMIKAZE_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"

class Enemy_Kamikaze : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	Animation fly;
	Path path;
	iPoint original_position;
	int y_transition;
	bool has_transitioned = false;
	int shots = 0;

public:

	Enemy_Kamikaze(int x, int y);
	~Enemy_Kamikaze();

	void Move();
	void OnCollision(Collider* collider);

};

#endif
