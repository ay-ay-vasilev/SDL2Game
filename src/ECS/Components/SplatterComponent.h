#pragma once
#include "ECS.h"
#include "Observer.h"

namespace ecs
{
	class HealthComponent;
	class SplatterComponent : public Component, public Observer
	{
	public:
		explicit SplatterComponent();
		virtual ~SplatterComponent() {}

		struct SplatterData
		{
			Vector2D splatterPosition;

		};

		// Component
		void init() override;

		// Observer
		void onNotify(const std::string_view& observedEvent) override;

		bool inline needToCreateSplatter() const { return needSplatter; }
		void inline setNeedToCreateSplatter(bool value) { needSplatter = value; }

		SplatterData inline getSplatterData() const { return splatterData; }
	private:
		void createSplatter();
		std::shared_ptr<HealthComponent> healthComponent;

		bool needSplatter = false;
		SplatterData splatterData{};
	};
}