#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(std::string textureSheet, int x, int y) :
    xpos(x), ypos(y)
{
    objTexture = TextureManager::LoadTexture(textureSheet);
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
    xpos++;
    ypos++;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 30;
    srcRect.h = 54;
    
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w * 2;
    destRect.h = srcRect.h * 2;
}

void GameObject::Render()
{
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}
