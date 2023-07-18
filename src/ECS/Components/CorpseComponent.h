#pragma once
#include "ECS.h"

namespace ecs
{
	class CorpseComponent : public Component
	{
	public:
		CorpseComponent() {}

		const bool inline isCorpse() const { return corpse; }
		void inline setCorpse(bool value) { corpse = value; }
	private:
		bool corpse{ false };
	};
}