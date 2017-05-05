#ifndef __Bonus_H__
#define __Bonus_H__

#include "p2Point.h"
#include "Animation.h"
#include "ModuleParticles.h"

struct SDL_Texture;
struct Collider;



class Bonus
{
protected:

	Animation* animation = nullptr;
	Collider* collider = nullptr;
	uint sdl_clock;
	uint sdl_clock_start;
	iPoint direction = { 0,1 };
	
public:

	Bonus(int x, int y);
	virtual ~Bonus();

	const Collider* GetCollider() const;

	iPoint position;
	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);

};
#endif
