#pragma once
class Animation
{
public:
	Animation() : index(0), frames(0), speed(0), triggerFrames({}) {}
	Animation(const int index, const int frames, const int speed, const std::span<const int>& triggerFrames = {}, const std::string& actionName = "") :
		index(index),
		frames(frames), speed(speed),
		triggerFrames(triggerFrames.begin(), triggerFrames.end()),
		actionName(actionName) {}

	const int index;
	const int frames;
	const int speed;
	const std::vector<int> triggerFrames;
	const std::string actionName;
private:
};