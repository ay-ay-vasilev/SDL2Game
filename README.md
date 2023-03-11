# SDL2Game

#TODO:

Stuff to remember:
1. UI
2. Health
3. Shooting tower
4. Containers for UI elements
5. Scene manager to "switch" levels

My plan:
- Add a simple AI system.
	* Refactor AI component ALMOST (remove magic num + smart pointer for entity)
	* AI movement (refine: REDO entity getting in CollisionSystem)
	* Default behavior
	* Notice/aggro range

- Ranged weapon for player (after JSON weapon refactor).
- Add a simple menu screen + UI system.
- Add simple sounds.
- Add an inventory system.
- Add skills.
- Add rotate to Transform.
- Improve movement and controls.
- Tweak attack movement.
- Implement attack combos, remove attack holding ?

Maintenance:
- Smart pointers for entities.
- Smart pointers for observers.
- Fix scale.
- Add map json.
- Fix Tile-Collider logic.
- Clean up in constants and settings.json.
- Refactor actor json to an entity json ?
- Maybe optimize the RenderSystem? (limit to only drawing entities in the view of the camera + move sorting logic from update).
- JSON files for weapons.
- Better WeaponComponent implementation (add an object for weapon stats).
- i-frames ?

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