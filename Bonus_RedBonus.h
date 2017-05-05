#ifndef __BONUS_REDBONUS_H__
#define __BONUS_REDBONUS_H__

#include "Bonus.h"
#include "Path.h"
#include "Globals.h"

class Bonus_RedBonus : public Bonus
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation fly;
	Path path;
	iPoint original_position;

public:

	Bonus_RedBonus(int x, int y);
	~Bonus_RedBonus();

	void Move();
};

#endif

