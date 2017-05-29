#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_SPRITES 25

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_SPRITES];
	int* animation = nullptr;

private:
	float current_frame;
	int loops = 0;
	int animation_size = 0;
	int start_frame = 0;

public:

	bool SetUp(int first_x, int first_y, int sprite_w, int sprite_h, int columns, int total_sprites, 
		char* animation_sequence)
	{
		if (total_sprites > MAX_SPRITES)
			return false;

		if (animation_sequence != "")
			++animation_size;

		for (char* tmp = animation_sequence; *tmp != '\0'; ++tmp) {
			if (*tmp == ',') ++animation_size;
		}

		if (animation != nullptr)
			delete[] animation;

		animation = new int[animation_size];

		animation[0] = 0; //initialize first value
		for (int i = 0; i < animation_size; ++animation_sequence) {
			if (*animation_sequence == ',') {
				++i;
				animation[i] = 0;
			}
			else if (*animation_sequence == '\0') {
				++i;
			}
			else {
				animation[i] *= 10;
				animation[i] += *animation_sequence - '0';
			}
		}

		frames[0].x = first_x; frames[0].y = first_y;
		frames[0].w = sprite_w; frames[0].h = sprite_h;

		for (int i = 1; i < total_sprites; ++i) {
			frames[i].x = frames[i - 1].x + sprite_w;
			frames[i].y = frames[i - 1].y;
			frames[i].w = sprite_w;
			frames[i].h = sprite_h;
			if (frames[i].x >= frames[0].x + columns * sprite_w) {
				frames[i].x = frames[0].x;
				frames[i].y = frames[i - 1].y + sprite_h;
			}
		}

		return true;
	}



	~Animation() {
		//CleanUp();
	}

	void PushBack(const SDL_Rect& rect) {
		frames[animation_size++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame > animation_size - 1)
		{
			current_frame = (loop) ? start_frame : animation_size - 1;
			loops++;
		}

		return frames[animation[(int)current_frame]];
	}

	SDL_Rect& CurrentFrame() 
	{
		return frames[animation[(int)current_frame]];
	}

	void SetFrame(int frame)
	{
		current_frame = frame;
	}

	bool Finished() const
	{
		return loops > 0;
	}


	void LoopStart(int start) {
		start_frame = start;
	}

	void Reset()
	{
		current_frame = 0;
		loops = 0;
	}

	void CleanUp() 		
	{
		if (animation != nullptr)
			delete[] animation;
		animation = nullptr;
		animation_size = 0;
		loops = 0;
		current_frame = 0;
	}
};

#endif