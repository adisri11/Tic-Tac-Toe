#include <iostream>
#include <climits>
#include <limits>
#include "GameState.h"
#include "Graph.h"
#include "LinkedList.h"
#include <cstdlib>
#include <ctime>

using namespace std;

struct Move {
    int x;
    int y;
    Move(){
        x = -1;
        y = -1;
    }
    Move(int row, int col){
        x = row;
        y = col;
    }
};

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
                   bestScore = max(bestScore, score);
                  
                   alpha = max(alpha, bestScore);
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
                  
                   bestScore = min(bestScore, score);
                  
                   beta = min(beta, bestScore);
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

Move bestMoveE(GameState& state, int player) {
   // Arrays to store possible moves
   Move possibleMoves[100];
   int moveScores[100];
   int moveCount = 0;

   // Seed random number generator
   srand(time(NULL));

   // Easy AI strategy:
   // 1. Prefer random moves
   // 2. Avoid strategically important moves
   // 3. Make occasional intentional mistakes

   // Find all possible moves
   for (int i = 0; i < state.size; i++) {
       for (int j = 0; j < state.size; j++) {
           if (state.grid[i][j] == -1) {
               possibleMoves[moveCount] = Move(i, j);
               
               // Give random, lower scores to introduce unpredictability
               moveScores[moveCount] = rand() % 10;
               
               moveCount++;
           }
       }
   }
  
   if (moveCount == 0) {
       return Move();
   }

   // Occasionally make a deliberately bad move (25% chance)
   if (rand() % 4 == 0) {
       int badMoveIndex = rand() % moveCount;
       return possibleMoves[badMoveIndex];
   }

   // Prioritize edges and center less strategically
   int centerRow = state.size / 2;
   int centerCol = state.size / 2;
   
   // Slightly lower score for center and edge moves
   for (int i = 0; i < moveCount; i++) {
       // Lower score for center move
       if (possibleMoves[i].x == centerRow && possibleMoves[i].y == centerCol) {
           moveScores[i] -= 5;
       }
       
       // Lower score for edge moves
       if (possibleMoves[i].x == 0 || possibleMoves[i].x == state.size - 1 ||
           possibleMoves[i].y == 0 || possibleMoves[i].y == state.size - 1) {
           moveScores[i] -= 3;
       }
   }

   // Find the worst score (to avoid good moves)
   int worstScore = INT_MAX;
   for (int i = 0; i < moveCount; i++) {
       if (moveScores[i] < worstScore) {
           worstScore = moveScores[i];
       }
   }

   // Collect moves with the worst score
   Move worstMoves[100];
   int worstMoveCount = 0;
   
   for (int i = 0; i < moveCount; i++) {
       if (moveScores[i] == worstScore) {
           worstMoves[worstMoveCount++] = possibleMoves[i];
       }
   }
  
   // Randomly choose from the worst moves
   if (worstMoveCount > 0) {
       int randomIndex = rand() % worstMoveCount;
       return worstMoves[randomIndex];
   }
  
   // Fallback to a random move
   return possibleMoves[rand() % moveCount];
}

Move bestMoveH(GameState& state, int player) {
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


int main() {
    char playAgain;
    do {
        int gridSize;
        char gameMode;
        char firstPlayer;
        char ai;

        cout << "Welcome to Tic-Tac-Toe!" << endl;

        // Grid Size Selection
        while(true){
            cout << "Choose desired grid size (3, 4, 5): ";
            if(cin >> gridSize){
                if(gridSize >= 3 && gridSize <= 5){
                    break;
                } else {
                    cout << "Invalid grid size. Please choose from 3, 4, 5." << endl;
                }
            } else{
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid. Please enter 3, 4, 5: " << endl;
            }
        }

        // Game Mode Selection
        cout << "Choose a game mode: " << endl;
        cout << "P: Player vs Player" << endl;
        cout << "A: Player vs AI" << endl;
        cout << "Which would you like to play? ";
        cin >> gameMode;

        while (gameMode != 'P' && gameMode != 'p' && gameMode != 'A' && gameMode != 'a') {
            cout << "Invalid choice. Please choose P or A: ";
            cin >> gameMode;
        }

        // AI Difficulty Selection
        cout << "Choose your difficulty of AI:  " << endl;
        cout << "E: Easy" << endl;
        cout << "H: Hard" << endl;
        cin >> ai;

        while (ai != 'E' && ai != 'e' && ai != 'H' && ai != 'h'){
            cout << "Invalid choice. Please choose E or H: ";
            cin >> ai;
        }

        // First Player Selection
        cout << "Which player goes first? (X = player 1/AI or O Player 2): ";
        cin >> firstPlayer;

        while (firstPlayer != 'X' && firstPlayer != 'x' && firstPlayer != 'O' && firstPlayer != 'o') {
            cout << "Invalid choice. Please enter X or O: ";
            cin >> firstPlayer;
        }

        // Game Initialization
        GameState game(gridSize);

        if (gameMode == 'A' || gameMode == 'a') {
            game.currentTurn = (firstPlayer == 'X' || firstPlayer == 'x') ? 0 : 1;
        } else {
            game.currentTurn = (firstPlayer == 'X' || firstPlayer == 'x') ? 0 : 1;
        }

        bool playerVsAI = (gameMode == 'A' || gameMode == 'a');
        bool playerVPlayer = (gameMode == 'P' || gameMode == 'p');

        cout << game << endl;

        // Game Loop
        while (!game.done) {
            cout << "Current Turn: " << (game.currentTurn ? "O" : "X") << endl;

            if (playerVsAI && game.currentTurn == 0) {
                if(ai == 'E' || ai == 'e'){
                    Move aiMove = bestMoveE(game, 0);
                    game.play(aiMove.x, aiMove.y);
                    game.currentTurn = 1;
                    cout << "AI plays at (" << aiMove.x << ", " << aiMove.y << ")" << endl;
            } else if (ai == 'H' || ai == 'h'){
                Move aiMove = bestMoveH(game, 0);
                game.play(aiMove.x, aiMove.y);
                game.currentTurn = 1;
                cout << "AI plays at (" << aiMove.x << ", " << aiMove.y << ")" << endl;
            }
            }
            else if (playerVsAI && game.currentTurn == 1) {
                int x, y;
                while (true) {
                    cout << "Enter the coordinates for Player O (e.g., x y): ";
                    if (cin >> x >> y) {
                        if (x >= 0 && x < gridSize && y >= 0 && y < gridSize) {
                            if (game.play(x, y)) {
                                break; // Valid move
                            } else {
                                cout << "Invalid move. That cell is already occupied. Try again." << endl;
                            }
                        } else {
                            cout << "Invalid coordinates. Enter values between 0 and " << gridSize - 1 << "." << endl;
                        }
                    } else {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter two numbers separated by a space." << endl;
                    }
                }
                
                game.currentTurn = 0;
            }
            else if (playerVPlayer) {
                int x, y;
                cout << "Enter coordinates (" << (game.currentTurn ? "O" : "X") << ")  EX.) x y: ";
                cin >> x >> y;

                while (!game.play(x, y)) {
                    cout << "Invalid move. Try again: ";
                    cin >> x >> y;
                }
            }

            cout << game << endl;

            if (game.hasWon(0)) {
                cout << "Player X Wins!" << endl;
                break;
            }
            else if (game.hasWon(1)) {
                cout << "Player O Wins!" << endl;
                break;
            }
            else if (game.done) {
                cout << "It's a tie!" << endl;
                break;
            }
        }

        // Play Again Prompt
        cout << "Do you want to play again? (Y/N): ";
        cin >> playAgain;

        // Input validation for play again
        while (playAgain != 'Y' && playAgain != 'y' && playAgain != 'N' && playAgain != 'n') {
            cout << "Invalid input. Do you want to play again? (Y/N): ";
            cin >> playAgain;
        }

    } while (playAgain == 'Y' || playAgain == 'y');

    cout << "Thanks for playing Tic-Tac-Toe!" << endl;

    return 0;
}