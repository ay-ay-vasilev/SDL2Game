# SDL2Game

#TODO:

Stuff to remember:
1. UI
2. Health
3. Shooting tower
4. Containers for UI elements
5. Scene manager to "switch" levels

My plan:
- Add sword attack.
- Add a simple AI system.
- Add a simple health system.
- Add a simple menu screen + UI system.
- Add simple sounds.
- Add an inventory system.
- Add skills.
- Add rotate to Transform
- Improve movement and controls.

Maintenance:
- Clean up in constants and settings.json.
- Add projectile json.
- Add map json.
- Untie collider size from sprite size.
- Fix player and enemy walk animation.
- Fix scale.

- Refactor actor json to an entity json ?
- Maybe optimize the RenderSystem? (limit to only drawing entities in the view of the camera + move sorting logic from update)
- Fix Tile-Collider logic