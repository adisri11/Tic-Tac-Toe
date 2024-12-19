#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <GL/gl.h>
#include "AppController.h"
#include "Game.h"
#include "Button.h"

class Controller : public AppController {
Game game;

public:

    Controller(){
        game.AIOn();
        game.playerOFirst();
        game.start();
    }

    void leftMouseDown(float x, float y) {
        game.handleMouseClick(x, y);
    };

    void render(){
        game.draw();
    }
};

#endif