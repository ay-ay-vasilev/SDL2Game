# SDL2Game

#TODO:

Stuff to remember:
1. UI
2. Health
3. Shooting tower
4. Containers for UI elements
5. Scene manager to "switch" levels

My plan:
- Adapt the sword for the goblin actor and the shortsword for the human actor.
	* Draw in aseprite DONE
	* Export spritesheets of hands DONE
	* Rename front and back spritesheets to 0 and 1 DONE
	* Implement adding and removing Sprite objects when equipping weapons (adding WeaponComponents) DONE
	* Adapt attack animations for different weapon types and play animations through ActorComponent (not SpriteComponent) DONE
	* Refactor actor hands asset structure similarly to body
	* Add unarmed animations
	* Add weapon swapping


- Add changing weapons.
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
- Restrict access to SpriteComponent for KeyboardComponent and AIComponentBasicEnemy
- Specific weapon stats for actors in weapon json files?
- Add string ids to Sprite vectors ?
- Remove unused CameraSystem and add CameraComponent (after RenderSystem refactor)
- JSON files for AI.
- Add map json.
- Fix Tile-Collider logic.
- Clean up in constants and settings.json.
- Refactor actor json to an entity json ?
- Maybe optimize the RenderSystem? (limit to only drawing entities in the view of the camera + move sorting logic from update).
- Better WeaponComponent implementation (add an object for weapon stats).
- i-frames ?
- Delete textures properly ?

Future plans:
- Different weapons
- Different clothes
- Customization for characters
- Actor randomization
- Player randomization
- Looting
- Inventory system
- Death animation
- Hit feedback