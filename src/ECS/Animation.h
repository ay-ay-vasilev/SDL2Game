#pragma once

class Animation
{
public:
	Animation() {}
	Animation(int index, int frames, int speed) : index(index), frames(frames), speed(speed)
	{
	}

	int index;
	int frames;
	int speed;
private:
};