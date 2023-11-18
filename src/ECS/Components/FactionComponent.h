#pragma once
#include "ECS.h"
#include <unordered_map>

namespace ecs
{
	enum class eFactionRelation
	{
		Hostile = -50,
		Neutral = 0,
		Friendly = 50
	};

	class FactionComponent : public Component
	{
	public:
		explicit FactionComponent(const std::string faction) : faction(faction) {}
		const std::string inline getFaction() const { return faction; }

		void inline setFactionRelation(const std::string factionName, int relationVal) { factionRelations[factionName] = relationVal; }
		bool inline checkIfFactionHostile(const std::string factionName)
		{
			return checkIfHostile(getFactionRelation(factionName));
		}
		bool inline checkIfFactionFriendly(const std::string factionName)
		{
			return checkIfFriendly(getFactionRelation(factionName));
		}

	private:
		const int inline getFactionRelation(const std::string factionName)
		{
			if (factionName == faction) return static_cast<int>(eFactionRelation::Friendly);
			return factionRelations.count(factionName) ? factionRelations[factionName] : static_cast<int>(eFactionRelation::Neutral);
		}

		bool inline checkIfHostile(const int relationValue) const { return relationValue <= static_cast<int>(eFactionRelation::Hostile); }
		bool inline checkIfFriendly(const int relationValue) const { return relationValue >= static_cast<int>(eFactionRelation::Friendly); }

		std::string faction;
		std::unordered_map<std::string, int> factionRelations;
	};
}