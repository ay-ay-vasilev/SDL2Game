#include "ActorSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SpriteOutlineComponent.h"
#include "ColliderComponent.h"
#include "ActorComponent.h"
#include "HitboxComponent.h"
#include "HealthComponent.h"
#include "AIComponentBasicFighter.h"
#include "ArmorComponent.h"
#include "WeaponComponent.h"
#include "FactionComponent.h"

ecs::Entity* ecs::ActorSystem::instantiateActor(const Vector2D& pos, const std::string& filename)
{
	const auto actorData = Game::assets->getActorJson(filename);
	auto& actor(manager.addEntity());
	actor.addComponent<ecs::TransformComponent>
	(
		pos.x * manager.getScale(), pos.y * manager.getScale(),
		actorData.value("width", 0), actorData.value("height", 0),
		manager.getScale(), actorData["speed"]
	);
	actor.addComponent<ecs::SpriteComponent>(actorData["sprite_data"], true);
	actor.addComponent<ecs::SpriteOutlineComponent>(actorData["sprite_data"].contains("outline") ? actorData["sprite_data"]["outline"] : nullptr);
	actor.addComponent<ecs::ActorComponent>(filename);
	actor.addComponent<ecs::ColliderComponent>(filename, actorData["collider_rect"]);
	actor.addComponent<ecs::HitboxComponent>(filename, actorData["hitbox_rect"]);
	actor.addComponent<ecs::HealthComponent>(actorData["health"]);
	actor.addComponent<ecs::ArmorComponent>();
	actor.addComponent<ecs::AIComponentBasicFighter>();
	actor.addComponent<ecs::FactionComponent>(actorData.contains("faction") ? actorData["faction"] : "neutral");

	actor.addGroup(Game::eGroupLabels::ACTORS);

	return &actor;
}

void ecs::ActorSystem::equipWeapon(ecs::Entity& actor, const std::string& weaponName)
{
	if (!actor.hasComponent<ecs::ActorComponent>()) return;

	const std::string actorType = actor.getComponent<ecs::ActorComponent>()->getActorType();

	if (actor.hasComponent<ecs::WeaponComponent>())
	{
		if (actor.getComponent<ecs::WeaponComponent>()->getTag() == weaponName)
			return;
		actor.removeComponent<ecs::WeaponComponent>();
	}
	actor.addComponent<ecs::WeaponComponent>(weaponName, actorType);
}

void ecs::ActorSystem::equipArmor(ecs::Entity& actor, const std::string& armorName, const std::string& slotName)
{
	if (!actor.hasComponent<ecs::ActorComponent>()) return;

	const std::string actorType = actor.getComponent<ecs::ActorComponent>()->getActorType();

	if (actor.hasComponent<ecs::ArmorComponent>())
	{
		const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();
		armorComponent->equipArmorToSlot(armorName, actorType, slotName);
	}
}

void ecs::ActorSystem::unequipArmorPiece(ecs::Entity& actor, const std::string& slotName)
{
	if (actor.hasComponent<ecs::ArmorComponent>())
	{
		const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();
		armorComponent->unequipArmorFromSlot(slotName);
	}
}

void ecs::ActorSystem::unequipAllArmor(ecs::Entity& actor)
{
	if (actor.hasComponent<ecs::ArmorComponent>())
	{
		const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();
		armorComponent->unequipAllArmor();
	}
}


void ecs::ActorSystem::update()
{
	actors = manager.getGroup(Game::eGroupLabels::ACTORS);
}

void ecs::ActorSystem::draw()
{
}