#pragma once
#include "ECS.h"
#include "Observer.h"
#include "Subject.h"

class SpriteComponent;
class WeaponComponent;
class ActorComponent : public Component, public Observer, public Subject
{
public:
	explicit ActorComponent(const std::string& name);
	virtual ~ActorComponent() {}

	// Component
	void init() override;

	// Observer
	void onNotify(const std::string_view& observedEvent) override;

	void playAction(const std::string& actionName);

private:
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::weak_ptr<WeaponComponent> weaponComponent;
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> actionAnimationsByWeaponType;
};