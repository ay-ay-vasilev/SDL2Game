#pragma once
class Animation
{
public:
	Animation() : startFrame(0), animIndex(0), numOfFrames(0), animSpeed(0), triggerFrames({}) {}
	Animation(const int animIndex, const int numOfFrames, const int animSpeed, const std::span<const int>& triggerFrames = {}, const std::string& actionName = "", const int startFrame = 0) :
		startFrame(startFrame),
		animIndex(animIndex),
		numOfFrames(numOfFrames), animSpeed(animSpeed),
		triggerFrames(triggerFrames.begin(), triggerFrames.end()),
		actionName(actionName) {}

	const int inline getStartFrame() const { return startFrame; }
	const int inline getAnimIndex() const { return animIndex; }
	const int inline getNumOfFrames() const { return numOfFrames; }
	const int inline getAnimSpeed() const { return animSpeed; }
	const std::vector<int> inline getTriggerFrames() const{ return triggerFrames; }
	const std::string inline getActionName() const { return actionName; }

private:
	const int startFrame{ 0 };
	const int animIndex{ 0 };
	const int numOfFrames{ 0 };
	const int animSpeed;
	const std::vector<int> triggerFrames;
	const std::string actionName;
};