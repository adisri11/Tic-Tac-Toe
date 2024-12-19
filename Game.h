#ifndef GAME_H
#define GAME_H


#include "GameState.h"
#include "Graph.h"
#include "Button.h"
#include "LinkedList.h"
#include "Move.h"
#include <iostream>
#include <climits>
#include <limits>


enum SIZE { THREE, FOUR, FIVE };


class Game {
   Square **board;
   int count;
   bool playerX;
   bool AI;


   bool winnerX;
   bool winnerO;
   bool tieOutcome;
   bool done = false;
   bool hardAiClicked = false;
   bool easyAiClicked = false;


   bool filled;

   Button welcome;

   Button button3;
   Button button4;
   Button button5;

   Button easyAi;
   Button hardAi;

   Button player1;
   Button player2;

   Button playAgain;
   Button quit;

   Button tie;
   Button xwin;
   Button owin;

   Button first;
   Button XFirst;
   Button OFirst;
   Button OFirstAI;


int minimax(GameState& state, int depth, bool isMaximizing, int alpha, int beta, int player, int maxDepth) {
   if (depth >= maxDepth || state.done) {
       if (state.hasWon(0)) {
           return 100 - depth;
       }
       if (state.hasWon(1)) {
           return -100 - depth;
       }
       return 0;
   }




   int currentPlayer = isMaximizing ? 0 : 1;




   if (isMaximizing) {
       int bestScore = INT_MIN;
       for (int i = 0; i < state.size; i++) {
           for (int j = 0; j < state.size; j++) {
               if (state.grid[i][j] == -1) {
                   GameState nextState = state;
                   nextState.play(i, j);
                 
                   int score = minimax(nextState, depth + 1, false, alpha, beta, player, maxDepth);
                   bestScore = std::max(bestScore, score);
                 
                   alpha = std::max(alpha, bestScore);
                   if (beta <= alpha)
                       break;
               }
           }
           if (beta <= alpha)
               break;
       }
       return bestScore;
   } else {
       int bestScore = INT_MAX;
       for (int i = 0; i < state.size; i++) {
           for (int j = 0; j < state.size; j++) {
               if (state.grid[i][j] == -1) {
                   GameState nextState = state;
                   nextState.play(i, j);
                 
                   int score = minimax(nextState, depth + 1, true, alpha, beta, player, maxDepth);
                 
                   bestScore = std::min(bestScore, score);
                 
                   beta = std::min(beta, bestScore);
                   if (beta <= alpha)
                       break;
               }
           }
           if (beta <= alpha)
               break;
       }
       return bestScore;
   }
}


Move bestMove(GameState& state, int player) {
   Move possibleMoves[100];
   int moveScores[100];
   int moveCount = 0;




   int maxDepth;
   switch (state.size) {
       case 3:
           maxDepth = 9;
           break;
       case 4:
           maxDepth = 5;
           break;
       case 5:
           maxDepth = 3;
           break;
       default:
           maxDepth = 3;
   }




   for (int i = 0; i < state.size; i++) {
       for (int j = 0; j < state.size; j++) {
           if (state.grid[i][j] == -1) {
               GameState nextState = state;
               nextState.play(i, j);
             
               int moveScore = minimax(nextState, 0, false, INT_MIN, INT_MAX, player, maxDepth);
             
               possibleMoves[moveCount] = Move(i, j);
               moveScores[moveCount] = moveScore;
               moveCount++;
           }
       }
   }
 
   if (moveCount == 0) {
       return Move();
   }
 
   srand(time(NULL));




   int bestScore = INT_MIN;
   for (int i = 0; i < moveCount; i++) {
       if (moveScores[i] > bestScore) {
           bestScore = moveScores[i];
       }
   }




   Move bestMoves[100];
   int bestMoveCount = 0;
 
   for (int i = 0; i < moveCount; i++) {
       if (moveScores[i] == bestScore) {
           bestMoves[bestMoveCount++] = possibleMoves[i];
       }
   }
 
   if (bestMoveCount > 0) {
       int randomIndex = rand() % bestMoveCount;
       return bestMoves[randomIndex];
   }
 
   return possibleMoves[rand() % moveCount];
}



// Updated AIMove method
void AIMove() {
    if(hardAiClicked){
        if (!playerX) {
            // Convert the current board state to a GameState
            GameState currentState(count);
            for (int i = 0; i < count; i++) {
                for (int j = 0; j < count; j++) {
                    if (board[i][j].getPlayer() == PLAYER_X) {
                        currentState.grid[i][j] = 0;
                    } else if (board[i][j].getPlayer() == PLAYER_O) {
                        currentState.grid[i][j] = 1;
                    }
                }
            }




            // Get the best move
            Move aiMove = bestMove(currentState, PLAYER_O);
       
            // Make the move on the board
            board[aiMove.x][aiMove.y].playO();
            playerX = true;
        }


    } else if(easyAiClicked){
        // this might be a mistake
        if(winnerX== false || winnerO == false){
            if (!playerX) {
                for (int i = 0; i < count; i++) {
                    filled = false;
                    for(int j = 0; j < count; j++){
                        if(board[i][j].isEmpty()){
                            board[i][j].playO();
                            filled = true;
                            break;
                        }
                    }
                    if(filled == true){
                        checkWinner();
                    }
                    if(filled == true){
                        break;
                    }
                   
                }
                playerX = !playerX;
            }
        }
   
    }
    // if (!playerX) {
    //     // Convert the current board state to a GameState
    //     GameState currentState(count);
    //     for (int i = 0; i < count; i++) {
    //         for (int j = 0; j < count; j++) {
    //             if (board[i][j].getPlayer() == PLAYER_X) {
    //                 currentState.grid[i][j] = 0;
    //             } else if (board[i][j].getPlayer() == PLAYER_O) {
    //                 currentState.grid[i][j] = 1;
    //             }
    //         }
    //     }




    //     // Get the best move
    //     Move aiMove = bestMove(currentState, PLAYER_O);
       
    //     // Make the move on the board
    //     board[aiMove.x][aiMove.y].playO();
    //     playerX = true;
    // }
};


   void checkWinner(){
       //tie
       int column = 0;
       for(int i = 0; i < count; i++){
           for(int z = 0; z < count; z++){
               if(board[i][z].getPlayer() == PLAYER_X || board[i][z].getPlayer() == PLAYER_O){
                   column++;
               }
           }
           if(column == count * count){
               tieOutcome = true;
               done = true;
               break;
           }
       }








       //X wins
       for(int i = 0; i < count; i++){
           int xCount = 0;
           for(int z = 0; z < count; z++){
               if(board[i][z].getPlayer() == PLAYER_X){
                   xCount++;
               }
           }
           if(xCount == count){
               winnerX= true;
               done = true;
               break;
           }
       }








       for(int i = 0; i < count; i++){
           int xCount = 0;
           for(int z = 0; z < count; z++){
               if(board[z][i].getPlayer() == PLAYER_X){
                   xCount++;
               }
           }
           if(xCount == count){
               winnerX= true;
               done = true;
               break;
           }
       }








       int xCount = 0;
       for(int i = 0; i < count; i++){
           if(board[i][i].getPlayer() == PLAYER_X){
               xCount++;
           }
           if(xCount == count){
               winnerX = true;
               done = true;
               xCount = 0;
               break;
           }
       }
       int xCounts = 0;
       for(int i = 0; i < count; i++){
           if(board[count - 1 - i][i].getPlayer() == PLAYER_X){
               xCounts++;
           }
           if(xCounts == count){
               winnerX = true;
               done = true;
               xCounts = 0;
               break;
           }
       }








       //O wins
       for(int i = 0; i < count; i++){
           int xCount = 0;
           for(int z = 0; z < count; z++){
               if(board[i][z].getPlayer() == PLAYER_O){
                   xCount++;
               }
           }
           if(xCount == count){
               winnerO = true;
               done = true;
               break;
           }
       }








       for(int i = 0; i < count; i++){
           int xCount = 0;
           for(int z = 0; z < count; z++){
               if(board[z][i].getPlayer() == PLAYER_O){
                   xCount++;
               }
           }
           if(xCount == count){
               winnerO = true;
               done = true;
               break;
           }
       }








       xCount = 0;
       for(int i = 0; i < count; i++){
           if(board[i][i].getPlayer() == PLAYER_O){
               xCount++;
           }
           if(xCount == count){
               winnerO = true;
               done = true;
               xCount = 0;
               break;
           }
       }
       xCounts = 0;
       for(int i = 0; i < count; i++){
           if(board[count - 1 - i][i].getPlayer() == PLAYER_O){
               xCounts++;
           }
           if(xCounts == count){
               winnerO = true;
               done = true;
               xCounts = 0;
               break;
           }
       }
   }

   void init() {
       board = new Square*[count];
       for (int i = 0; i < count; i++) {
           board[i] = new Square[count];
       }








       float x = -0.9;
       float y = 0.9;
       float w = 1.5 / count;
       done = false;








       // Initialize your state variables
       for (int i = 0; i < count; i++) {
           x = -0.9;
           for (int j = 0; j < count; j++) {
               board[i][j] = Square(x, y, w);
               x += w;
           }
           y -= w;
       }
   }

   int screen;


public:
   Game()
   
       : welcome("Welcome to Tic Tac Toe!", -0.55, 0.9),
          button3("3 x 3", -0.55, 0.6), 
          button4("4 x 4", -0.15, 0.6),
          button5("5 x 5", 0.25, 0.6),
          player1("Single Player", -0.6, 0.3), 
          player2("Two Player", 0.1, 0.3),
          easyAi("Easy AI", -0.45, 0),
          hardAi("Hard AI", 0, 0),
          first("Who would you like to play first?", -0.75, -0.3),
          XFirst("X First", -0.45, -0.6),
          OFirst("O First", 0, -0.6),
          OFirstAI("O First (AI)", 0, -0.6),
          playAgain("Play Again", -0.25, -0.7),
          quit("Quit", 0.5, -0.7),
          xwin("X wins!", -0.2, 0.1),
          owin("O wins!", -0.2, 0.1),
          tie("Tie!", -0.1, 0.1),
         count(3), screen(1), playerX(true), AI(false)
         {
       init();




   }








   void AIOn() {
       AI = true;
       AIMove();
   }








   void AIOff() {
       AI = false;
   }








   void playerXFirst() {
       playerX = true;
   }








   void playerOFirst() {
       playerX = false;
   }








 








   void start() {
       if (AI) {
           for (int j = 0; j < count; j++) {
               if (board[0][j].isEmpty()) {
                   board[0][j].playO();
                   break;
               }
           }
           playerX = !playerX;
       }
   }








   void deselect() {
       button3.selected = false;
       button4.selected = false;
       button5.selected = false;

       player1.selected = false;
       player2.selected = false;

       easyAi.selected = false;
       hardAi.selected = false;

       XFirst.selected = false;
       OFirst.selected = false;
       OFirstAI.selected = false;

       playAgain.selected = false;
       quit.selected = false;
   }


   void handleMouseClick(float x, float y) {
       if (screen == 1){
           if (button3.contains(x, y)) {
               std::cout << "Will change to 3x3" << std::endl;
               resetBoard(3);
               playerOFirst();
               button4.selected = false;
               button5.selected = false;
           } else if (button4.contains(x, y)) {
               std::cout << "Will change to 4x4" << std::endl;
               resetBoard(4);
               playerOFirst();
               button3.selected = false;
               button5.selected = false;
           } else if (button5.contains(x, y)) {
               std::cout << "Will change to 5x5" << std::endl;
               resetBoard(5);
               playerOFirst();
               button3.selected = false;
               button4.selected = false;
           }

           if (player1.contains(x, y)) {
                AIOn();
                player2.selected = false;
           } else if (player2.contains(x, y)) {
                AIOff();
                player1.selected = false;
                easyAi.selected = false;
                hardAi.selected = false;
           }
            if(easyAi.contains(x, y)){
                easyAiClicked = true;
                hardAi.selected = false;
            } else if (hardAi.contains(x, y)){
                hardAiClicked = true;
                easyAi.selected = false;
            }

            if(XFirst.contains(x, y)){
                playerXFirst();
                resetBoard(count);
                deselect();
                screen = 2;
            } else if(OFirst.contains(x, y)){
                playerOFirst();
                resetBoard(count);
                deselect();
                screen = 2;
                if (AI){
                    AIMove();
                }
            }
         
       } else if (screen == 2){
           for (int i = 0; i < count; i++) {
               for (int j = 0; j < count; j++) {
                   if (board[i][j].contains(x, y) && board[i][j].isEmpty()) {
                       if (playerX) {
                           board[i][j].playX();
                       } else {
                           board[i][j].playO();
                       }
                       playerX = !playerX;
                       break;
                   }
               }
           }

            checkWinner();
            if(done == false){
                if(AI){
                    AIMove();
                }
            }

            checkWinner();
            if(winnerX || winnerO || tieOutcome){
                screen = 3;
                deselect();
            }

            if (playAgain.contains(x, y)){
                screen = 1;
                deselect();
                winnerX = false;
                winnerO = false;
                tieOutcome = false;
            } else if (quit.contains(x, y)){
                exit(0);
            }
       } else if (screen == 3){
           if (playAgain.contains(x, y)){
               screen = 1;
               deselect();
               winnerX = false;
               winnerO = false;
               tieOutcome = false;
           } else if (quit.contains(x, y)){
               exit(0);
           }
       }








     
     
   }








   void resetBoard(int newCount) {
       for (int i = 0; i < count; i++) {
           delete[] board[i];
       }
       delete[] board;




       




       count = newCount;
       init();
   }








   void draw() {
        if (screen == 1) {
            welcome.draw();
            button3.draw();
            button4.draw();
            button5.draw();
            if(button3.selected){
                player1.draw();
                player2.draw();
            } else if(button4.selected){
                player1.draw();
                player2.draw();
            } else if(button5.selected){
                player1.draw();
                player2.draw();
            }   

            if (player1.selected){
                easyAi.draw();
                hardAi.draw();
            } else if(player2.selected){
                first.draw();
                XFirst.draw();
                OFirst.draw();
            }

            if(easyAi.selected || hardAi.selected){
                first.draw();
                XFirst.draw();
                OFirstAI.draw();
            }
        } else if (screen == 2) {
            for (int i = 0; i < count; i++) {
                for (int j = 0; j < count; j++) {
                    board[i][j].draw();
                }
            }
            playAgain.draw();
            quit.draw();
        }
        if (screen == 3){
            playAgain.draw();
            quit.draw();
            if (winnerX){
                xwin.draw();
            } else if (winnerO){
                owin.draw();
            } else if (tieOutcome){
                tie.draw();
            }
        }
    }
};








#endif
