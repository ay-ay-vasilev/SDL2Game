#pragma once
class Animation
{
public:
	Animation() : index(0), frames(0), speed(0), frameWidth(0), frameHeight(0) {}
	Animation(int index, int frames, int speed) :
		index(index),
		frames(frames), speed(speed),
		frameWidth(0), frameHeight(0) {}
	Animation(int index, int frames, int speed, int frameWidth, int frameHeight) :
		index(index),
		frames(frames), speed(speed),
		frameWidth(frameWidth), frameHeight(frameHeight) {}

	int index;
	int frames;
	int speed;
	int frameWidth;
	int frameHeight;
private:
};