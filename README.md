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
    * DONE Remove weapon sprites from actor spritesheets.
    * DONE Add dedicated spritesheets for hands and weapons.
    * Rework the RenderSystem to account for different Z values of pushed sprites:
        - Refactor SpriteComponent to hold a vector of sprites with Z values and to sort the sprites before drawing them.
        - Make it so components with Sprite data would push their sprites to SpriteComponent, instead of drawing their sprites.
- Remove unused CameraSystem and add CameraComponent (after RenderSystem refactor)
- JSON files for AI.
- Add map json.
- Fix Tile-Collider logic.
- Clean up in constants and settings.json.
- Refactor actor json to an entity json ?
- Maybe optimize the RenderSystem? (limit to only drawing entities in the view of the camera + move sorting logic from update).
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