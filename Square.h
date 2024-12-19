#ifndef SQUARE_H
#define SQUARE_H


#include <GL/gl.h>
#include <cmath>


enum SquareState {EMPTY, PLAYER_X, PLAYER_O};


class Square {
    float x;
    float y;
    float size;
    SquareState state;


public:
    Square() {
        x = 0.0f;
        y = 0.0f;
        size = 0.6f;
        state = EMPTY;
    }


    Square(float x, float y, float size) {
        this->x = x;
        this->y = y;
        this->size = size;
        state = EMPTY;
    }


    bool isEmpty() {
        return state == EMPTY;
    }


    void playX() {
        state = PLAYER_X;
    }


    SquareState getPlayer() {
        return state;
    }


    bool isSelected(){
        return state;
    }


    void playO() {
        state = PLAYER_O;
    }


    void draw() {
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(2.0f);


        glBegin(GL_LINES);
            glVertex2f(x, y);
            glVertex2f(x + size, y);


            glVertex2f(x + size, y);
            glVertex2f(x + size, y - size);


            glVertex2f(x + size, y - size);
            glVertex2f(x, y - size);


            glVertex2f(x, y - size);
            glVertex2f(x, y);
        glEnd();


        if (state == PLAYER_X) {
            glBegin(GL_LINES);
                glVertex2f(x + 0.1f, y - 0.1f);
                glVertex2f(x + size - 0.1f, y - size + 0.1f);


                glVertex2f(x + size - 0.1f, y - 0.1f);
                glVertex2f(x + 0.1f, y - size + 0.1f);
            glEnd();
        } else if (state == PLAYER_O) {
            glBegin(GL_LINES);
                float inc = (2 * M_PI) / 60;
                for (float theta = 0; theta <= 2 * M_PI; theta += inc) {
                    glVertex2f((x + (size / 2)) + (size / 2 - 0.1f) * cos(theta), (y - (size / 2)) + (size / 2 - 0.1f) * sin(theta));
                    glVertex2f((x + (size / 2)) + (size / 2 - 0.1f) * cos(theta + inc), (y - (size / 2)) + (size / 2 - 0.1f) * sin(theta + inc));
                }
            glEnd();
        }
    }


    bool contains(float mx, float my) {
        if (mx >= x && mx <= x + size && my <= y && my >= y - size) {
            return true;
        } else {
            return false;
        }
    }
};


#endif
