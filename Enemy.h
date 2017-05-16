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

enum EnemyState {
	REGULAR = 0,
	HURT,
	SHOOTING

};

class Enemy
{
protected:
	unit_type type;
	Animation* animation = nullptr;
	Animation animation_hurt;
	Animation animation_shooting;
	Animation shadow;
	Collider* collider = nullptr;
	uint sdl_clock;
	uint sdl_clock_start;
	iPoint direction = { 0,1 };
	bool shot = false;
	EnemyState state = REGULAR;

	iPoint collider_offset;


public:
	iPoint position;

public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Damaged() {};
	virtual void Shoot(iPoint origin);
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);

public:
	int hitpoints;
	bool to_delete = false;
};

#endif
