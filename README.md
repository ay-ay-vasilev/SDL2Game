# SDL2Game

#TODO:

Stuff to remember:
1. UI
2. Health
3. Shooting tower
4. Containers for UI elements
5. Scene manager to "switch" levels

My plan:
- Ranged weapon for player (after weapons refactor).
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
- Weapons refactor:
    * DONE Make a json file and a directory for weapons.
    * DONE Add weapons to entities through systems with a dedicated method (not on creation).
    * Remove weapon sprites from actor spritesheets.
    * Add dedicated spritesheets for hands and weapons.
    * Rework the RenderSystem to account for different Z values of pushed sprites:
        - Add a RenderComponent that stores sprites and their Z values.
        - Make it so SpriteComponent and WeaponComponent push their sprites to RenderComponent, instead of drawing their sprites.
- JSON files for AI.
- Add map json.
- Fix Tile-Collider logic.
- Clean up in constants and settings.json.
- Refactor actor json to an entity json ?
- Maybe optimize the RenderSystem? (limit to only drawing entities in the view of the camera + move sorting logic from update).
- Better WeaponComponent implementation (add an object for weapon stats).
- i-frames ?
- CameraSystem
- AIComponent direction stuff
- AssetManager small refactor ?

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