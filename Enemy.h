#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"
#include "ModuleParticles.h"

struct SDL_Texture;
struct Collider;

enum unit_type {
	GROUND = 3,
	AIRBORNE = 6
};

class Enemy
{
protected:
	unit_type type;
	Animation* animation = nullptr;
	Collider* collider = nullptr;
	uint sdl_clock;
	uint sdl_clock_start;
	iPoint direction = { 0,1 };
	bool shot = false;

public:
	iPoint position;

public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Shoot(iPoint origin);
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
};

#endif
