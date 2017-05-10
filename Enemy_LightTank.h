#ifndef __ENEMY_LIGHTTANK_H__
#define __ENEMY_LIGHTTANK_H__


#include "Enemy.h"
#include "Path.h"
#include "Globals.h"

enum LIGHTTANK_SUBTYPE
{
	NORMAL = 2,
	VARIATION1,
	VARIATION2
};

	
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
	int subtype;
	int shots = 0;

public:

	Enemy_LightTank(int x, int y, int subtype);
	void Draw(SDL_Texture * sprites);
	~Enemy_LightTank();

	void Move();
	void OnCollision(Collider * collider);
};




#endif