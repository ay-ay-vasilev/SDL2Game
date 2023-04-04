#pragma once
class Animation
{
public:
	Animation() : animIndex(0), numOfFrames(0), animSpeed(0), triggerFrames({}) {}
	Animation(const int animIndex, const int numOfFrames, const int animSpeed, const std::span<const int>& triggerFrames = {}, const std::string& actionName = "") :
		animIndex(animIndex),
		numOfFrames(numOfFrames), animSpeed(animSpeed),
		triggerFrames(triggerFrames.begin(), triggerFrames.end()),
		actionName(actionName) {}

	const int getAnimIndex() const { return animIndex; }
	const int getNumOfFrames() const { return numOfFrames; }
	const int getAnimSpeed() const { return animSpeed; }
	const std::vector<int> getTriggerFrames() const{ return triggerFrames; }
	const std::string getActionName() const { return actionName; }

private:
	const int animIndex;
	const int numOfFrames;
	const int animSpeed;
	const std::vector<int> triggerFrames;
	const std::string actionName;
};