#include "FactionSystem.h"
#include "AssetManager.h"
#include "FactionComponent.h"

void ecs::FactionSystem::init()
{
	const auto factionData = assets::getGeneralDataJson("factions")["factions"];
	for (const auto faction : factionData)
	{
		std::unordered_map<std::string, int> relations;
		const auto relationsData = faction["relations"];
		for (const auto relation : relationsData)
		{
			relations[relation["faction"]] = relation["value"];
		}
		factions[faction["id"]] = relations;
	}
}

void ecs::FactionSystem::update(double delta)
{
	updateRelations();
}

void ecs::FactionSystem::draw()
{
}

void ecs::FactionSystem::updateRelations()
{
	auto entitiesWithFactions = manager.getEntitiesWithComponent<ecs::FactionComponent>();

	for (const auto entityWithFaction : entitiesWithFactions)
	{
		auto factionComponent = entityWithFaction->getComponent<ecs::FactionComponent>();
		
		if (factions.count(factionComponent->getFaction()))
		{
			for (const auto [factionName, relationVal] : factions[factionComponent->getFaction()])
				factionComponent->setFactionRelation(factionName, relationVal);
		}
	}
}