#ifndef __Enemy1_H__
#define __Enemy1_H__

#include "Enemy.h"
#include "Path.h"

class Enemy1 : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation fly;
	Path path;
	iPoint original_position;

public:

	Enemy1(int x, int y);

	void Move();
};

#endif
