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
	Animation turret;
	Path path;
	iPoint original_position;
	int x_offset, y_offset = 0;

public:

	Enemy_LightTank(int x, int y);
	void Draw(SDL_Texture * sprites);
	~Enemy_LightTank();

	void Move();
	void OnCollision(Collider * collider);
};




#endif