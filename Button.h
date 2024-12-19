#ifndef BUTTON_H
#define BUTTON_H

#include <GL/freeglut.h>
#include <string>

struct Button {
    std::string text;
    float x;
    float y;
    float w;
    float h;
    bool pressed;
    bool selected;

    Button() {
        x = 0.0f;
        y = 0.0f;
        w = 0.4f;
        h = 0.2f;
        text = "Button";
        pressed = false;
        selected = false;
    }

    Button(std::string text, float x, float y) {
        this->text = text;
        this->x = x;
        this->y = y;
        w = 0.4f;
        h = 0.2f;
        pressed = false;
        selected = false;
    }

    void draw() {
        float textWidth = 0.0f;
        for (int i = 0; i < text.length(); i++) {
            textWidth += glutBitmapWidth(GLUT_BITMAP_9_BY_15, text[i]);
        }

        float padding = 0.06f;
        textWidth = 2.0f * (textWidth / 400.0f);
        w = textWidth + padding;

        if (selected) {
            glColor3f(0.8f, 0.8f, 0.8f);
        } else {
            glColor3f(1.0f, 1.0f, 1.0f);
        }

        glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y - h);
        glVertex2f(x, y - h);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x + w, y);

        glVertex2f(x + w, y);
        glVertex2f(x + w, y - h);

        glVertex2f(x + w, y - h);
        glVertex2f(x, y - h);

        glVertex2f(x, y - h);
        glVertex2f(x, y);
        glEnd();

        if (pressed) {
            glRasterPos2f(x + (padding / 2) - 0.01f, y - (h / 2) - 0.045f);
        } else {
            glRasterPos2f(x + (padding / 2), y - (h / 2) - 0.025f);
        }

        for (int i = 0; i < text.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
        }
    }

    void output(int x, int y, float r, float g, float b, void* font, const std::string& str) {
        glColor3f(r, g, b);  // Set the text color
        glRasterPos2f(x, y); // Set the text position on the screen


        int len = (int)str.length();  // Use the string's length() method instead of strlen
        for (int i = 0; i < len; i++) {
            glutBitmapCharacter(font, str[i]);  // Draw each character
        }
    }


    bool contains(float mx, float my) {
        if (mx >= x && mx <= x + w && my <= y && my >= y - h) {
            selected = true;
            return true;
        } else {
            return false;
        }
    }
};

#endif
