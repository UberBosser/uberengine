#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "uberengine.h"


bool GameObject::operator==(const GameObject *gameObject) const {
    return (this == gameObject);
}

bool GameObject::collideRect(GameObject gameObject) {
    if (rect.y + rect.h <= gameObject.rect.y) {
        return false;
    }
    if (rect.y >= gameObject.rect.y + gameObject.rect.h) {
        return false; 
    }
    if (rect.x + rect.w <= gameObject.rect.x) {
        return false;
    }
    if (rect.x >= gameObject.rect.x + gameObject.rect.w) {
        return false; 
    }
    return true;
}

int GameObject::getPosX() {
    return rect.x;
}

int GameObject::getPosY() {
    return rect.y;
}

int GameObject::getWidth() {
    return rect.w;
}

int GameObject::getHeight() {
    return rect.h;
}

SDL_Renderer* GameObject::getRenderer() {
    return renderer;
}

void GameObject::addChild(GameObject *g) {
    children.push_back(g);
    childrenSize = children.size();
}

void GameObject::removeChild(GameObject *g) {
    for (int i = 0; i < childrenSize; i++) {
        if (*children[i] == g) {
            children.erase(children.begin() + i);
            childrenSize = children.size();
        }
    }
}

GameObject* GameObject::getChild(int i) {
    return children[i];
}

void GameObject::update() {}

void GameObject::updateChildren() {
    for (int i = 0; i < childrenSize; i++) {
        children[i]->update();
        if (!children[i]->children.empty()) {
            children[i]->update();
        }
    }
}

void GameObject::draw() {}

void GameObject::draw(int offsetX, int offsetY) {}

void GameObject::drawChildren() {
    for (int i = 0; i < childrenSize; i++) {
        children[i]->draw();
        if (!children[i]->children.empty()) {
            children[i]->drawChildren(); 
        } 
    }
}

void GameObject::drawChildren(int offsetX, int offsetY) {
    for (int i = 0; i < childrenSize; i++) {
        children[i]->draw(offsetX, offsetY);
        if (!children[i]->children.empty()) {
            children[i]->drawChildren(offsetX, offsetY); 
        }
    }
}


SpriteObject::SpriteObject(SDL_Renderer *r) {
    renderer = r;
}

SpriteObject::SpriteObject(GameObject *p) {
    parent = p;
    renderer = parent->getRenderer();
}

void SpriteObject::createSurface(int x, int y, int w, int h) {
    surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void SpriteObject::createSurface(int x, int y, int w, int h, Uint32 c) {
    surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    SDL_FillRect(surface, NULL, c);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); 
}

void SpriteObject::createSurface(int x, int y, const char *i) {
    surface = IMG_Load(i);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void SpriteObject::update() {}

void SpriteObject::draw() {
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &pivot, flip);
}

void SpriteObject::draw(int offsetX, int offsetY) {
    rect.x -= offsetX;
    rect.y -= offsetY;
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &pivot, flip);
}


TextObject::TextObject(SDL_Renderer *r) {
    renderer = r;
    color.r = 255;
    color.g = 255;
    color.b = 255;
}

TextObject::TextObject(GameObject *p) {
    parent = p;
    renderer = parent->getRenderer();
    color.r = 255;
    color.g = 255;
    color.b = 255;
}

void TextObject::createSurface(int x, int y, const char *t, int s, const char *f) {
    font = TTF_OpenFont(f, 24);
    surface = TTF_RenderText_Blended(font, t, color);
    rect = surface->clip_rect;
    rect.x = x;
    rect.y = y;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void TextObject::updateText(const char *t) {
    surface = TTF_RenderText_Blended(font, t, color);
    rect = surface->clip_rect;
    pivot.x = rect.w/2;
    pivot.y = rect.h/2;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); 
}

void TextObject::update() {}

void TextObject::draw() {
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &pivot, flip);
}

void TextObject::draw(int offsetX, int offsetY) {
    rect.x -= offsetX;
    rect.y -= offsetY;
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &pivot, flip);
}


GameCamera::GameCamera(int w, int h) {
    rect.w = w;
    rect.h = h;
}

void GameCamera::setTarget(GameObject *gameObject) {
    target = gameObject;
}

void GameCamera::setSize(int w, int h) {
    rect.w = w;
    rect.h = h;
}

void GameCamera::update() {
    rect.x = (target->getPosX() + target->getWidth()/2) - rect.w/2;
    rect.y = (target->getPosY() + target->getHeight()/2) - rect.h/2; 
}


GameManager::GameManager() {
    quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    window = SDL_CreateWindow("UberEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1366, 768, SDL_WINDOW_FULLSCREEN_DESKTOP);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    screen = SDL_GetWindowSurface(window);
    screenWidth = 1366;
    screenHeight = 768;
    SDL_SetWindowIcon(window, IMG_Load("Assets/icon.png"));
}

GameManager::GameManager(int x, int y) {
    quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    window = SDL_CreateWindow("UberEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_FULLSCREEN_DESKTOP);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    screen = SDL_GetWindowSurface(window);
    screenWidth = x;
    screenHeight = y;
    SDL_SetWindowIcon(window, IMG_Load("Assets/icon.png")); 
}

int GameManager::getScreenWidth() {
    return screenWidth;
}

int GameManager::getScreenHeight() {
    return screenHeight;
}

SDL_Renderer* GameManager::getRenderer() {
    return renderer;
}

void GameManager::events() {
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true; 
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
            quit = true;}
        }
    }
}

void GameManager::update() {}

void GameManager::draw() {}

void GameManager::capFps() {
    if ((1000/60) > SDL_GetTicks() - startTick) {
        SDL_Delay(1000/60 - (SDL_GetTicks() - startTick));
    }
}

void GameManager::loop() {
    while (quit == false) {
        startTick = SDL_GetTicks();
        events();
        update();
        SDL_RenderClear(renderer);
        draw();
        SDL_RenderPresent(renderer);
        capFps();
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
