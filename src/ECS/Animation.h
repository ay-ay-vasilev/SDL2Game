#pragma once

class Animation
{
public:
	Animation() : index(0), frames(0), speed(0) {}
	Animation(int index, int frames, int speed) : index(index), frames(frames), speed(speed) {}

	int index;
	int frames;
	int speed;
private:
};