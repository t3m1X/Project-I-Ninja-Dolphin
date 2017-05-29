#ifndef __PATH_H__
#define __PATH_H__

#include "p2Point.h"
#define MAX_STEPS 25

struct Step
{
	uint frames = 1;
	fPoint speed;
	Animation* animation = nullptr;
};

class Path
{
public:
	bool loop = true;
	Step steps[MAX_STEPS];
	fPoint accumulated_speed = {0.0f, 0.0f};
	

private:
	uint current_frame = 0;
	uint last_step = 0;
	int loops = 0;
	int start_step = 0;

public:

	void PushBack(fPoint speed, uint frames, Animation* animation = nullptr)
	{
		steps[last_step].animation = animation;
		steps[last_step].frames = frames;
		steps[last_step++].speed = speed;
	}

	iPoint GetCurrentPosition(Animation** current_animation = nullptr)
	{
		current_frame += 1;

		uint count = 0;
		bool need_loop = true;
		for(uint i = 0; i < last_step; ++i)
		{
			count += steps[i].frames;
			if(current_animation != nullptr)
				*current_animation = steps[i].animation;
			if(count >= current_frame)
			{
				accumulated_speed += steps[i].speed;
				need_loop = false;
				break;
			}
		}

		if (need_loop) {
			loops++;
			if (loop)
				current_frame = start_step;
		}

		return iPoint((int)accumulated_speed.x, (int)accumulated_speed.y);
	}

	void LoopStart(int start) 
	{
		if (start < MAX_STEPS)
			start_step = start;
	}

	bool IsFinished() 
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0;
	}

};

#endif // __PATH_H__