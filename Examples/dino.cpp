#include <iostream>
#include <UE/UE_core.h>
#include <UE/UE_utils.h>


class Player : public SpriteObject {
    public:
        Player(GameInfo* g) : SpriteObject(g) {
            loadImage(0, 0, 48, 48, "Assets/Dinos/dinoSheet.png", 2);
            rect.x = 0;
            rect.y = 0;
            speed = 3;
            idle = true;
            runningFrame = 4;

            position = new FontObject(g, {255, 0, 0, 255});
            position->loadFont(100, 100, "(0, 0)", 24, "Assets/PressStart2P.ttf");
        }

        void animation() {
            if (idle) {
                if (runningTime != 0) { 
                    runningTime = 0;
                    runningFrame = 4;
                    speed = 3;
                }
                if (idleTime % 10 == 0) {
                    if (idleFrame != 3) {
                        idleFrame++;
                    } else {
                        idleFrame = 0;
                    }
                    changeFrame(idleFrame);
                    idleTime = 0;
                }
                idleTime++;
            } else {
                if (idleTime != 0) { 
                    idleTime = 0;
                }
                if (runningTime % 10 == 0 && runningTime < 120) {
                    if (runningFrame != 9) {
                        runningFrame++;
                    } else {
                        runningFrame = 4;
                    }
                    changeFrame(runningFrame);
                } else if (runningTime == 120) {
                    runningFrame = 17;
                } else if (runningTime % 10 == 0) {
                    if (speed == 3) { 
                        speed = 5;
                    }
                    if (runningFrame != 23) {
                        runningFrame++;
                    } else {
                        runningFrame = 18;
                        runningTime = 120;
                    }
                    changeFrame(runningFrame);
                }
                runningTime++;
            }
        }

        void update() {
            const Uint8* keys = GET_KEYS();
            idle = true;
            if (keys[KEY_A]) {
                rect.x += -speed;
                idle = false;
                if (flipped == true) {
                    flip(SDL_FLIP_HORIZONTAL);
                    flipped = false;
                }
            } 
            if (keys[KEY_D]) {
                rect.x += speed;
                idle = false;
                if (flipped == false) {
                    flip(SDL_FLIP_NONE);
                    flipped = true;
                }
            }
            if (keys[KEY_W]) {
                rect.y += -speed;
                idle = false;
            }
            if (keys[KEY_S]) {
                rect.y += speed;
                idle = false;
            }
            animation();
            position->updateText("(" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ")");
        }

        void draw() {
            drawTexture();
            position->draw();
        }

        ~Player() {
            delete position;
        }

    private:
        int speed;
        bool idle;
        int idleTime;
        int idleFrame;
        int runningTime;
        int runningFrame;
        bool flipped;

        FontObject* position;
};


class Game : public GameManager {
    public:
        Game() : GameManager() { 
            p = new Player(getGameInfo());
        }

        void update() {
            p->update();
        }

        void draw() {
            p->draw();
        }

        ~Game() {
            delete p;
        }

    private:
        Player *p;
};


int main() {
    Game g;
    g.loop();
}
