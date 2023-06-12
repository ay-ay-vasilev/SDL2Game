#pragma once
#include "ECS.h"
#include "Observer.h"

namespace ecs
{
	class HealthComponent;
	class SplatterComponent : public Component, public Observer
	{
	public:
		explicit SplatterComponent(const nlohmann::json& splatterData);
		virtual ~SplatterComponent() {}

		struct SplatterSettings
		{
			Vector2D splatterPosition = {0, 0};
			float intensity = 0.f;
			std::pair<float, float> radiusRange = {0, 0};
			std::pair<float, float> offsetRange = { 0, 0 };
			std::pair<int, int> redRange = { 0, 0 };
			std::pair<int, int> greenRange = { 0, 0 };
			std::pair<int, int> blueRange = { 0, 0 };
			std::pair<int, int> alphaRange = { 0, 0 };
		};

		// Component
		void init() override;

		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		bool inline needToCreateSplatter() const { return needSplatter; }
		void inline setNeedToCreateSplatter(bool value) { needSplatter = value; }

		SplatterSettings inline getSplatterSettings() const { return splatterSettings; }
	private:
		void createSplatter();
		std::shared_ptr<HealthComponent> healthComponent;

		bool needSplatter = false;
		SplatterSettings splatterSettings{};
	};
}