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
	* Add an attack animation DONE
	* Add an AttackComponent
	* Add signals to attack animations (start signal, hit signal, end signal)
	* Add a HitboxComponent

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
- Fix human walk animation.
- Fix scale.
- Refactor actor json to an entity json ?
- Maybe optimize the RenderSystem? (limit to only drawing entities in the view of the camera + move sorting logic from update)
- Fix Tile-Collider logic