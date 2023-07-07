#include "ActorSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SpriteOutlineComponent.h"
#include "TintComponent.h"
#include "ColliderComponent.h"
#include "ActorComponent.h"
#include "KeyboardComponent.h"
#include "HitboxComponent.h"
#include "HealthComponent.h"
#include "AIComponentBasicFighter.h"
#include "ArmorComponent.h"
#include "DamageColliderComponent.h"
#include "WeaponComponent.h"
#include "FactionComponent.h"
#include "CameraComponent.h"
#include "ShadowComponent.h"
#include "CorpseComponent.h"
#include "HitParticleComponent.h"
#include "SplatterComponent.h"
#include "DebugParticleComponent.h"
#include "AimComponent.h"

#include <random>

void ecs::ActorSystem::init()
{
	const auto actorsEquipmentData = assets::getGeneralDataJson("actor_equipment");
	for (auto& actorEntry : actorsEquipmentData["armor"].items())
	{
		const std::string actorName = actorEntry.key();
		std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> actorEquipmentMap;

		for (auto& equipmentEntry : actorEntry.value().items())
		{
			const std::string slotName = equipmentEntry.key();
			
			auto equipmentList = equipmentEntry.value();
			std::unordered_map<std::string, std::vector<std::string>> equipmentListWithColors;
			for (auto equipmentItem : equipmentList)
			{
				if (equipmentItem.is_string())
				{
					equipmentListWithColors[equipmentItem] = { "default" };
				}
				else
				{
					std::vector<std::string> colors;
					for (const auto color : equipmentItem["colors"])
					{
						colors.emplace_back(color);
					}
					equipmentListWithColors[equipmentItem["name"]] = colors;
				}
			}

			actorEquipmentMap.emplace(slotName, equipmentListWithColors);
		}
		actorArmors.emplace(actorName, actorEquipmentMap);
	}

	for (auto& [actorName, weaponVector] : actorsEquipmentData["weapons"].items())
	{
		actorWeapons.emplace(actorName, weaponVector);
	}

	const auto actorsCustomizationData = assets::getGeneralDataJson("customization");
	for (auto& actorEntry : actorsCustomizationData.items())
	{
		const std::string actorName = actorEntry.key();
		for (auto& spriteGroup : actorEntry.value().items())
		{
			const std::string groupName = spriteGroup.key();
			for (const auto& sprite : spriteGroup.value()["sprites"])
			{
				std::vector<std::string> textures;
				for (const auto& texture : sprite["textures"]) textures.push_back(texture);
				auto customizationSprite = ActorCustomizationSprite(sprite["slot"], textures, sprite["z"]);
				actorCustomizations[actorName][groupName].push_back(customizationSprite);
			}

			if (!spriteGroup.value().contains("colors")) continue;

			for (const auto& color : spriteGroup.value()["colors"])
			{
				actorCustomizationColors[actorName][groupName].push_back(color);
			}
		}
	}
}

void ecs::ActorSystem::update(double delta)
{
	actors = manager.getGroup(Game::eGroupLabels::ACTORS);
}

void ecs::ActorSystem::draw()
{
}

ecs::Entity* ecs::ActorSystem::instantiateActor(const Vector2D& pos, const std::string& filename)
{
	const auto actorData = assets::getActorJson(filename);
	auto& actor(manager.addEntity());
	actor.addComponent<ecs::TransformComponent>
	(
		pos.x * manager.getScale(), pos.y * manager.getScale(),
		actorData.value("width", 0), actorData.value("height", 0),
		manager.getScale(), actorData["speed"]
	);
	actor.addComponent<ecs::SpriteComponent>(actorData["sprite_data"], true);
	actor.addComponent<ecs::SpriteOutlineComponent>(actorData["sprite_data"].contains("outline") ? actorData["sprite_data"]["outline"] : nullptr);
	actor.addComponent<ecs::TintComponent>(actorData["sprite_data"].contains("tints") ? actorData["sprite_data"]["tints"] : nullptr);
	actor.addComponent<ecs::HealthComponent>(actorData["health"]);
	actor.addComponent<ecs::ActorComponent>(filename);
	actor.addComponent<ecs::ShadowComponent>(actorData["sprite_data"].contains("shadow") ? actorData["sprite_data"]["shadow"] : nullptr);
	actor.addComponent<ecs::ColliderComponent>(filename, actorData["collider_rect"]);
	actor.addComponent<ecs::HitboxComponent>(filename, actorData["hitbox_rect"]);
	actor.addComponent<ecs::ArmorComponent>();
	actor.addComponent<ecs::AIComponentBasicFighter>();
	actor.addComponent<ecs::FactionComponent>(actorData.contains("faction") ? actorData["faction"] : "neutral");
	actor.addComponent<ecs::CorpseComponent>();
	actor.addComponent<ecs::DamageColliderComponent>();
	actor.addComponent<ecs::WeaponComponent>();
	actor.addComponent<ecs::HitParticleComponent>(actorData["hit_particle"]);
	if (actorData.contains("splatter")) actor.addComponent<ecs::SplatterComponent>(actorData["splatter"]);

	actor.addGroup(Game::eGroupLabels::ACTORS);

	return &actor;
}

ecs::Entity* ecs::ActorSystem::instantiatePlayer(const Vector2D& pos, const std::string& filename)
{
	const auto playerData = assets::getActorJson(filename);
	auto& player(manager.addEntity());
	player.addComponent<ecs::TransformComponent>
		(
			pos.x * manager.getScale(), pos.y * manager.getScale(),
			playerData.value("width", 0), playerData.value("height", 0),
			manager.getScale(), playerData["speed"]
		);
	player.addComponent<ecs::SpriteComponent>(playerData["sprite_data"], true);
	player.addComponent<ecs::SpriteOutlineComponent>(playerData["sprite_data"].contains("outline") ? playerData["sprite_data"]["outline"] : nullptr);
	player.addComponent<ecs::TintComponent>(playerData["sprite_data"].contains("tints") ? playerData["sprite_data"]["tints"] : nullptr);
	player.addComponent<ecs::HealthComponent>(playerData["health"]);
	player.addComponent<ecs::ActorComponent>(filename);
	player.addComponent<ecs::ShadowComponent>(playerData["sprite_data"].contains("shadow") ? playerData["sprite_data"]["shadow"] : nullptr);
	player.addComponent<ecs::KeyboardComponent>();
	//player.addComponent<ecs::AIComponentBasicFighter>();
	player.addComponent<ecs::ColliderComponent>(filename, playerData["collider_rect"]);
	player.addComponent<ecs::HitboxComponent>(filename, playerData["hitbox_rect"]);
	player.addComponent<ecs::ArmorComponent>();
	player.addComponent<ecs::FactionComponent>(playerData.contains("faction") ? playerData["faction"] : "neutral");
	player.addComponent<ecs::CameraComponent>();
	player.addComponent<ecs::CorpseComponent>();
	player.addComponent<ecs::DamageColliderComponent>();
	player.addComponent<ecs::AimComponent>(playerData.contains("aim_data") ? playerData["aim_data"] : nullptr);
	player.addComponent<ecs::WeaponComponent>();
	player.addComponent<ecs::HitParticleComponent>(playerData["hit_particle"]);
	if (playerData.contains("splatter")) player.addComponent<ecs::SplatterComponent>(playerData["splatter"]);

	if (playerData.contains("debug"))
	{
		player.addComponent<ecs::DebugParticleComponent>(playerData["debug"]["particle"]);
	}

	player.addGroup(Game::eGroupLabels::PLAYERS);
	equipWeapon(player, "unarmed");
	return &player;
}

void ecs::ActorSystem::equipWeapon(ecs::Entity& actor, const std::string& weaponName)
{
	if (!actor.hasComponent<ecs::ActorComponent>() || !actor.hasComponent<ecs::WeaponComponent>()) return;

	actor.getComponent<ecs::WeaponComponent>()->equipWeapon(weaponName);
}

void ecs::ActorSystem::equipArmor(ecs::Entity& actor, const std::string& armorName, const std::string& slotName, const std::string& colorName)
{
	if (!actor.hasComponent<ecs::ActorComponent>() || !actor.hasComponent<ecs::ArmorComponent>()) return;
	const std::string actorType = actor.getComponent<ecs::ActorComponent>()->getActorType();

	const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();
	armorComponent->equipArmorToSlot(armorName, actorType, slotName, colorName);
}

void ecs::ActorSystem::unequipArmorPiece(ecs::Entity& actor, const std::string& slotName)
{
	if (!actor.hasComponent<ecs::ArmorComponent>()) return;

	const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();
	armorComponent->unequipArmorFromSlot(slotName);
}

void ecs::ActorSystem::unequipAllArmor(ecs::Entity& actor)
{
	if (!actor.hasComponent<ecs::ArmorComponent>()) return;

	const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();
	armorComponent->unequipAllArmor();
}

void ecs::ActorSystem::equipRandomArmor(ecs::Entity& actor)
{
	if (!actor.hasComponent<ecs::ArmorComponent>() || !actor.hasComponent<ecs::ActorComponent>()) return;

	const std::string actorType = actor.getComponent<ecs::ActorComponent>()->getActorType();
	const auto armorComponent = actor.getComponent<ecs::ArmorComponent>();

	std::random_device rd;
	std::mt19937 gen(rd());

	for (auto& [slotName, equipmentMap] : actorArmors[actorType])
	{
		if (!equipmentMap.empty())
		{
			std::uniform_int_distribution<> dis(0, static_cast<int>(equipmentMap.size()) - 1);
			const int equipmentIndex = dis(gen);
			auto it = std::next(equipmentMap.begin(), equipmentIndex);

			auto equipmentItem = *it;
			if (equipmentItem.first != "none")
			{
				std::uniform_int_distribution<> dis(0, static_cast<int>(equipmentItem.second.size()) - 1);
				const int colorIndex = dis(gen);
				armorComponent->equipArmorToSlot(equipmentItem.first, actorType, slotName, equipmentItem.second[colorIndex]);
			}
		}
	}
}

void ecs::ActorSystem::equipRandomWeapon(ecs::Entity& actor)
{
	if (!actor.hasComponent<ecs::ActorComponent>()) return;

	const std::string actorType = actor.getComponent<ecs::ActorComponent>()->getActorType();
	if (actorWeapons[actorType].empty()) return;

	const auto weaponsVector = actorWeapons[actorType];

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, static_cast<int>(weaponsVector.size()) - 1);

	const int weaponIndex = dis(gen);
	const auto weaponName = weaponsVector[weaponIndex] == "none" ? "unarmed" : weaponsVector[weaponIndex];

	equipWeapon(actor, weaponName);
}

void ecs::ActorSystem::addRandomCustomization(ecs::Entity& actor)
{
	if (!actor.hasComponent<ecs::ActorComponent>()) return;

	const auto actorComponent = actor.getComponent<ecs::ActorComponent>();
	const std::string actorType = actorComponent->getActorType();

	auto& customization = actorCustomizations[actorType];
	auto& colors = actorCustomizationColors[actorType];
	std::string color = "";

	if (customization.empty()) return;

	std::random_device rd;
	std::mt19937 gen(rd());

	for (const auto& spriteGroup : customization)
	{
		const auto spriteGroupName = spriteGroup.first;
		color.clear();
		if (colors.count(spriteGroupName))
		{
			std::uniform_int_distribution<> colorDistribution(0, static_cast<int>(colors[spriteGroupName].size()) - 1);
			color = colors[spriteGroupName][colorDistribution(gen)];
		}

		for (const auto sprite : spriteGroup.second)
		{
			std::uniform_int_distribution<> surfacesDistribution(0, static_cast<int>(sprite.availableSurfaces.size()) - 1);
			const auto surfaceId = sprite.availableSurfaces[surfacesDistribution(gen)];

			if (surfaceId == "none") continue;

			if (!color.empty()) actorComponent->addSprite(sprite.slotName, surfaceId, sprite.z, color);
			else actorComponent->addSprite(sprite.slotName, surfaceId, sprite.z);
		}
	}
}