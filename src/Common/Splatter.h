#pragma once
#include "Vector2D.h"

class Splatter
{
public:
	Splatter() = default;
	Splatter
	(
		const Vector2D& splatterCenter,
		const int splatterRadius,
		const float intensity,
		const std::pair<int, int>& redRange,
		const std::pair<int, int>& greenRange,
		const std::pair<int, int>& blueRange,
		const std::pair<int, int>& alphaRange
	) :
		splatterCenter(splatterCenter),
		splatterRadius(splatterRadius),
		intensity(intensity),
		redRange(redRange),
		greenRange(greenRange),
		blueRange(blueRange),
		alphaRange(alphaRange)
	{}
	~Splatter() {};

	const Vector2D inline getSplatterCenter() const { return splatterCenter; }
	const int inline getSplatterRadius() const { return splatterRadius; }
	const float inline getIntensity() const { return intensity; }
	const std::pair<int, int> inline getRedRange() const { return redRange; }
	const std::pair<int, int> inline getGreenRange() const { return greenRange; }
	const std::pair<int, int> inline getBlueRange() const { return blueRange; }
	const std::pair<int, int> inline getAlphaRange() const { return alphaRange; }

	void inline setSplatterCenter(const Vector2D& splatterCenter) { this->splatterCenter = splatterCenter; }

private:
	Vector2D splatterCenter = Vector2D();
	int splatterRadius = 0;
	float intensity = 1.f;

	std::pair<int, int> redRange{ 0, 0 };
	std::pair<int, int> greenRange{ 0, 0 };
	std::pair<int, int> blueRange{ 0, 0 };
	std::pair<int, int> alphaRange{ 0, 0 };
};