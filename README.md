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
- Ranged weapon for player.
- Add a simple menu screen + UI system.
- Add simple sounds.
- Add an inventory system.
- Add skills.
- Add rotate to Transform.
- Improve movement and controls.
- Tweak attack movement.
- Implement attack combos, remove attack holding ?

Maintenance:
- Fix scale.
- Add map json.
- Fix Tile-Collider logic
- Smart pointers for components ?
- Clean up in constants and settings.json.
- Refactor actor json to an entity json ?
- Maybe optimize the RenderSystem? (limit to only drawing entities in the view of the camera + move sorting logic from update)
- JSON files for weapons
- Better WeaponComponent implementation (add an object for weapon stats)
- Better death handling
- i-frames ?

Future plans:
- Different weapons
- Different clothes
- Customization for characters
- Actor randomization
- Player randomization
- Looting
- Inventory system