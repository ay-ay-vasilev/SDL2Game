#pragma once
class Animation
{
public:
	Animation() : index(0), frames(0), speed(0), frameWidth(0), frameHeight(0), triggerFrames({}) {}
	Animation(const int index, const int frames, const int speed, const int frameWidth = 0, const int frameHeight = 0, const std::span<const int>& triggerFrames = {}) :
		index(index),
		frames(frames), speed(speed),
		frameWidth(frameWidth), frameHeight(frameHeight),
		triggerFrames(triggerFrames.begin(), triggerFrames.end()) {}

	const int index;
	const int frames;
	const int speed;
	const int frameWidth;
	const int frameHeight;
	const std::vector<int> triggerFrames;
private:
};