#ifndef GAME_STATE_H
#define GAME_STATE_H




#include <iostream>
#include "Graph.h"
#include "LinkedList.h"
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




    friend class GameState;




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












struct Vec{
    int x;
    int y;




    Vec(){
        x = 0;
        y = 0;
    }




    Vec(int x, int y){
        this->x = x;
        this->y = y;
    }




    void set(int x, int y){
        this->x = x;
        this->y = y;
    }
};
















inline std::ostream& operator<<(std::ostream& os, const Vec& v){
    os << "(" << v.x << ", " << v.y << ")";




    return os;
}








struct GameState{
    int** grid;
    bool currentTurn;
    int size;
    int turnCount;




    bool done;
    Vec lastMove;












    GameState(int size = 7){
        this->size = size;
        currentTurn = 0;
        turnCount = 0;
        done = false;




        lastMove.set(-1, -1);




        grid = new int*[size];




        for (int i = 0; i < size; i++){
            grid[i] = new int[size];
            for (int j = 0; j < size; j++){
                grid[i][j] = -1;
            }
        }
    }




    GameState(const GameState& other){
        size = other.size;
        currentTurn = other.currentTurn;
        turnCount = other.turnCount;
        done = other.done;
        lastMove = other.lastMove;




        grid = new int*[size];




        for (int i = 0; i < size; i++){
            grid[i] = new int[size];
            for (int j = 0; j < size; j++){
                grid[i][j] = other.grid[i][j];
            }
        }
    }




    bool operator==(const GameState& other){
        bool sizeMatch = size == other.size;
        bool currentTurnMatch = currentTurn == other.currentTurn;
        bool turnCountMatch = turnCount == other.turnCount;
        bool doneMatch = done == other.done;
        bool lastMoveMatch = lastMove.x == other.lastMove.x && lastMove.y == other.lastMove.y;
        if (sizeMatch && currentTurnMatch && turnCountMatch && doneMatch && lastMoveMatch){




            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    if (grid[i][j] != other.grid[i][j]){
                        return false;
                    }
                }
            }




            return true;
        }
        else{
            return false;
        }
    }




    GameState& operator=(const GameState& other){
        currentTurn = other.currentTurn;
        turnCount = other.turnCount;
        done = other.done;
        lastMove = other.lastMove;




        if (size == other.size){
            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    grid[i][j] = other.grid[i][j];
                }
            }
        }
        else{
            size = other.size;
            for (int i = 0; i < size; i++){
                delete[] grid[i];
            }
            delete[] grid;




            grid = new int*[size];




            for (int i = 0; i < size; i++){
                grid[i] = new int[size];
                for (int j = 0; j < size; j++){
                    grid[i][j] = other.grid[i][j];
                }
            }
        }




        return *this;
    }




    int maxReward(Vertex<GameState>* node, int player) {
        if (node->edgeList.size() == 0) {
            if (node->data.hasWon(player)) {
                return 100;  // AI wins
            } else if (node->data.hasWon(!player)) {
                return -100;  // Opponent wins
            } else {
                return 0;  // Tie or not finished
            }
        } else {
            int reward = maxReward(node->edgeList[0]->to, player);
            for (int i = 1; i < node->edgeList.size(); i++) {
                int curr = maxReward(node->edgeList[i]->to, player);
                if (node->data.currentTurn == player) {
                    // AI's turn, maximize reward
                    if (curr > reward) {
                        reward = curr;
                    }
                } else {
                    // Opponent's turn, minimize reward
                    if (curr < reward) {
                        reward = curr;
                    }
                }
            }
            return reward;
        }
}
























//     int maxReward(Vertex<GameState>* node, int player){
//     if (node->edgeList.size() == 0){
//         if (node->data.hasWon(player)){
//             return 100;
//         }
//         else if (node->data.hasWon(!player)){
//             return -100;
//         }
//         else{
//             return 0;
//         }
//     }
//     else{
//         int reward = maxReward(node->edgeList[0]->to, player);
//         for (int i = 1; i < node->edgeList.size(); i++){
//             int curr = maxReward(node->edgeList[i]->to, player);
//             if (node->data.currentTurn == player){
//                 if (curr > reward){
//                     reward = curr;
//                 }
//             }
//             else {
//                 if (curr < reward){
//                     reward = curr;
//                 }
//             }
//         }
//         return reward;
//     }
// }








   




    bool hasWon(int player){
        for (int i = 0; i < size; i++){
            bool winRow = true;
            for (int j = 0; j < size; j++){
                if (grid[i][j] != player){
                    winRow = false;
                    break;
                }
            }
            if (winRow){
                return true;
            }
        }
        for (int i = 0; i < size; i++){
            bool winCol = true;
            for (int j = 0; j < size; j++){
                if (grid[j][i] != player){
                    winCol = false;
                    break;
                }
            }
            if (winCol){
                return true;
            }
        }




        bool winDiag = true;
        for (int i = 0; i < size; i++){
            if (grid[i][i] != player){
                winDiag = false;
                break;
            }
        }
        if (winDiag){
            return true;
        }
       
        bool winAntiDiag = true;
        for (int i = 0; i < size; i++){
            if (grid[i][size-1-i] != player){
                winAntiDiag = false;
                break;
            }
        }
        if (winAntiDiag){
            return true;
        }




        return false;
    }












// moves the player
    bool play(int x, int y){
        if (grid[x][y] != -1){
            return false;
        }




        grid[x][y] = currentTurn;
        currentTurn = !currentTurn;
        turnCount++;
        lastMove.set(x, y);




        if (turnCount == size * size){
            done = true;
        }
        else if (hasWon(0) || hasWon(1)){
            done = true;
        }




        return true;
    }




    ~GameState(){
        for (int i = 0; i < size; i++){
            delete[] grid[i];
        }
        delete[] grid;
    }
};




template <class T>
inline std::ostream& operator<<(std::ostream& os, const GameState& state){
    os << "   ";
    for (int j = 0; j < state.size; j++){
        os << " " << j << "  ";
    }
    os << std::endl;
    os << "   ";
    for (int j = 0; j < state.size; j++){
        os << "--- ";
    }
    os << std::endl;
    for (int i = 0; i < state.size; i++){
        os << i << " ";
        for (int j = 0; j < state.size; j++){
            char c = ' ';
            if (state.grid[i][j] == 0){
                c = 'X';
            }
            else if (state.grid[i][j] == 1){
                c = 'O';
            }
            os << "| " << c << " ";
            if (j == state.size - 1) os << "|";
        }
        os << std::endl << "   ";
        for (int j = 0; j < state.size; j++){
            os << "--- ";
        }
        os << std::endl;
    }




    return os;
}




#endif
