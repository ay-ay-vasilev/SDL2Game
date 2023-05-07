# SDL2Game

#TODO:

Stuff to remember:
1. UI
2. Containers for UI elements
3. Scene manager to "switch" levels

My plan:
- Ranged weapon for player.
- Add a simple menu screen + UI system.
- Add simple sounds.
- Add an inventory system.
- Add skills.
- Add rotate to Transform.
- Improve movement and controls.
- Tweak attack movement.
- Implement attack combos, remove attack holding ?
- Default behavior for enemies.

Maintenance:
- Refactor colors - implement palette swapping
	- Move eyes to a different group ?
	- Move footwear to its own slot
	- Add gloves
	- Redo human and skeleton armor
	- Add colors to armor + add variation to humans and skeles
- JSON files for AI.
- Add map json.
- Fix Tile-Collider logic.
- Clean up in constants and settings.json.
- Better WeaponComponent implementation (add an object for weapon stats).
- i-frames ?
- Specific weapon stats for actors in weapon json files?
- Refactor ECS.h (uninline some methods, inline explicitly the rest)
- Refactor the debug draw methods
- Refactor FactionSystem (make it a manager instead ? avoid duplication but worsen encapsulation)
- Refactor AssetManager (remove useless references and remove string_view from maps)
- Deal with strings and string_views

Future plans:
- Add a second container for Sprites that should not be outlined? (sword swings)
- Customization for characters
- Looting
- Inventory system
- Death animation
- Hit feedback
- Hit cooldown ?
- Add body parts and body parts damage and armor ?
- More animations for AI actors ?
- Add armor condition ?