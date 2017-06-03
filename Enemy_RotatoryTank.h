#ifndef __ENEMY_ROTATORYTANK_H__
#define __ENEMY_ROTATORYTANK_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"

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
	int shot_angle = 0;
	int turret_angle = 360;
	bool rotating = false;

public:

	Enemy_RotatoryTank(int x, int y);
	~Enemy_RotatoryTank();

	void Move();
	void Draw(SDL_Texture * sprites);
	void Shoot(iPoint origin, fPoint direction);
	void OnCollision(Collider* collider);
};


#endif
