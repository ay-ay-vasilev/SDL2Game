# SDL2Game

#TODO:

Stuff to remember:
1. UI
2. Health
3. Shooting tower
4. Containers for UI elements
5. Scene manager to "switch" levels

My plan:
- Add armor.
	* Remove head into a separate spritesheet DONE
	* Create an Armor class and draw clothing on actors DONE
	* Reparse actor spritesheets ALMOST
	* Refactor ArmorComponent!!! Add unequip method + optimize the equip method. Also calculate damage through ArmorComponent and fix damageReduction logic.
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
- Remove string_view reference parameters if they are casted to strings.
- Fix new warnings.
- Delete textures properly ?
- Add implementation files for Components.
- Remove unused CameraSystem and add CameraComponent (after RenderSystem refactor)
- JSON files for AI.
- Add map json.
- Fix Tile-Collider logic.
- Clean up in constants and settings.json.
- Refactor actor json to an entity json ?
- Maybe optimize the RenderSystem? (limit to only drawing entities in the view of the camera + move sorting logic from update).
- Better WeaponComponent implementation (add an object for weapon stats).
- i-frames ?
- Specific weapon stats for actors in weapon json files?

Future plans:
- Different clothes
- Customization for characters
- Actor randomization
- Player randomization
- Looting
- Inventory system
- Death animation
- Hit feedback
- Add ids to entities (UUID or simple ids?)
- Outline for actors?