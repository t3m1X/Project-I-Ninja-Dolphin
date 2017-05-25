#ifndef __ENEMY_BOSS_H__
#define __ENEMY_BOSS_H__

#include "Enemy.h"
#include "Path.h"
#include "Globals.h"


enum SHOT_DIR
{
	NO_DIR,
	LOW_LEFT_DIR,
	LEFT_DIR,
	UP_LEFT_DIR,
	UP_RIGHT_DIR,
	RIGHT_DIR,
	LOW_RIGHT_DIR,
	PLAYER_DIR
};

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
	SHOT_DIR typology = SHOT_DIR::NO_DIR;

public:

	Enemy_Boss(int x, int y);
	~Enemy_Boss();

	void Move();
	void Draw(SDL_Texture * sprites);
	void Shoot(iPoint origin, SHOT_DIR typology);
	void OnCollision(Collider* collider);
};


#endif
