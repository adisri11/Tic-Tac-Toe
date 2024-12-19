#ifndef MOVE_H
#define MOVE_H



struct Move {
    int x;      // Row coordinate of the move
    int y;      // Column coordinate of the move
    int score;  // Evaluation score of the move




    // Default constructor
    Move() {
        x = -1;  
        y = -1;  
        score = 0;  
    }




    // Constructor with row and column
    Move(int row, int col) {
        x = row;
        y = col;
        score = 0;  
    }




    // Constructor with row, column, and score
    Move(int row, int col, int moveScore) {
        x = row;
        y = col;
        score = moveScore;  
    }
};




#endif
