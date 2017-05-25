#ifndef __ENEMY_BOSS_H__
#define __ENEMY_BOSS_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"

class Enemy_Boss : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation walk;
	Animation turret;
	Path path;
	iPoint original_position;
	int shots = 0;

public:

	Enemy_Boss(int x, int y);
	~Enemy_Boss();

	void Move();
	void Draw(SDL_Texture * sprites);
	/*void Shoot(iPoint origin);*/
	void OnCollision(Collider* collider);
};


#endif
