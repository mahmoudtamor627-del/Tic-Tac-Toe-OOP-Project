#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

// ============================================================================
// ENUMS & CONSTANTS
// ============================================================================
enum Difficulty {
    EASY,
    HARD
};

// ============================================================================
// CLASS: Board
// Responsibilities: Manage game board state and validation
// ============================================================================
class Board {
private:
    vector<vector<char>> grid;
    const int size;

public:
    // Constructor: Initializes empty grid of given size (default 3)
    Board(int size = 3) : size(size) {
        grid.resize(size, vector<char>(size, ' '));
    }

    // Prints formatted board with coordinates and borders
    void display() const {
        // TODO (Member 3): Implement formatted output matching PDF specification:
        //    1   2   3
        // 1  X | O |  
        //   ---+---+---
        // 2    | X | O
        //   ---+---+---
        // 3  O |   | X
    }

    // Places symbol if move is valid, returns success status
    bool makeMove(int row, int col, char symbol) {
        // TODO (Member 2): Place symbol on grid if validMove and return true, else false
        return false;
    }

    // Checks if coordinates are valid and cell is empty (0-based indexing)
    bool isValidMove(int row, int col) const {
        // TODO (Member 2): Check boundaries [0, size-1] and grid[row][col] == ' '
        return false;
    }

    // Checks all win conditions (rows, columns, diagonals)
    bool checkWin(char symbol) const {
        // TODO (Member 3): Verify if 3 matching symbols exist in any row, column, or diagonal
        return false;
    }

    // Checks if all cells are occupied
    bool isFull() const {
        // TODO (Member 3): Check if no ' ' cells remain
        return false;
    }

    // Returns the symbol at specified coordinates
    char getCell(int row, int col) const {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            return grid[row][col];
        }
        return ' ';
    }

    // Clears all cells to empty state
    void reset() {
        // TODO (Member 2): Reset all cells in grid to ' '
    }

    // Returns the dimension of the board
    int getSize() const {
        return size;
    }
};

// ============================================================================
// ABSTRACT BASE CLASS: Player
// Responsibilities: Base class for player entities
// ============================================================================
class Player {
protected:
    string name;
    char symbol;

public:
    Player(const string& name, char symbol) : name(name), symbol(symbol) {}
    virtual ~Player() = default;

    // Pure virtual function to be implemented by derived classes
    virtual void getMove(int& row, int& col) = 0;

    string getName() const { return name; }
    char getSymbol() const { return symbol; }
    void setName(const string& newName) { name = newName; }
};

// ============================================================================
// CLASS: HumanPlayer (Derived from Player)
// Responsibilities: Represents human player input
// ============================================================================
class HumanPlayer : public Player {
public:
    HumanPlayer(const string& name, char symbol) : Player(name, symbol) {}

    void getMove(int& row, int& col) override {
        // Handled via Game::handleHumanMove for robust I/O recovery
    }
};

// ============================================================================
// CLASS: AIPlayer (Derived from Player)
// Responsibilities: Computer opponent implementation (Easy & Hard / Minimax)
// ============================================================================
class AIPlayer : public Player {
private:
    Difficulty difficulty;

public:
    AIPlayer(const string& name, char symbol, Difficulty difficulty = EASY)
        : Player(name, symbol), difficulty(difficulty) {
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    void setDifficulty(Difficulty newDifficulty) {
        difficulty = newDifficulty;
    }

    // Determines AI move based on difficulty level
    void getMove(int& row, int& col) override {
        // Overridden by getAIMove(const Board& board, int& row, int& col)
    }

    void getAIMove(const Board& board, int& row, int& col) {
        if (difficulty == EASY) {
            getRandomMove(board, row, col);
        } else {
            getBestMove(board, row, col);
        }
    }

    // Easy AI: Selects random valid move
    void getRandomMove(const Board& board, int& row, int& col) const {
        // TODO (Member 5): Collect all valid (empty) coordinates and pick one randomly
    }

    // Hard AI: Evaluates board state for scoring (win: +10, loss: -10, draw: 0)
    int evaluateBoard(const Board& board) const {
        // TODO (Member 6): Evaluate terminal state from this AI's perspective
        return 0;
    }

    // Minimax recursive algorithm
    int minimax(Board& board, int depth, bool isMaximizing) {
        // TODO (Member 6): Implement recursive Minimax game tree search
        return 0;
    }

    // Hard AI: Finds optimal move using Minimax
    void getBestMove(Board board, int& row, int& col) {
        // TODO (Member 6): Call minimax on all possible moves and pick the optimal one
    }
};

// ============================================================================
// CLASS: Game
// Responsibilities: Manage game flow and coordination
// ============================================================================
class Game {
private:
    Board board;
    Player* player1;
    Player* player2;
    Player* currentPlayer;

public:
    Game() : board(3), player1(nullptr), player2(nullptr), currentPlayer(nullptr) {}

    ~Game() {
        delete player1;
        delete player2;
    }

    // Main game entry point, controls overall flow
    void start() {
        // TODO (محمود - Team Leader):
        // 1. Loop displaying showMenu()
        // 2. Play rounds using currentPlayer, handleHumanMove, handleAIMove
        // 3. Check checkGameOver()
        // 4. Display displayResult()
        // 5. Ask for replay (y/n)
    }

    // Displays mode selection menu and handles user choice
    void showMenu() {
        // TODO (محمود - Team Leader):
        // 1. Player vs Player
        // 2. Player vs Computer (Easy)
        // 3. Player vs Computer (Hard)
        // 4. Exit
    }

    void setupPvP() {
        // TODO: Prompt for Player 1 & Player 2 names, initialize HumanPlayer instances
    }

    void setupPvC(Difficulty difficulty) {
        // TODO: Prompt for Human name, initialize AIPlayer with given difficulty
    }

    void switchPlayer() {
        // TODO (محمود - Team Leader): Toggle currentPlayer between player1 and player2
    }

    void handleHumanMove(Player* player) {
        // TODO (Member 4): Prompt player for (row, col) in 1-based format.
        // Validate ranges [1-3], check cin.fail(), recover input buffer, check board.isValidMove.
    }

    void handleAIMove(AIPlayer* aiPlayer) {
        // TODO (Member 5): Display AI thinking message, execute getAIMove, apply move to board
    }

    bool checkGameOver() {
        // TODO (محمود - Team Leader): Check if currentPlayer won or if board.isFull()
        return false;
    }

    void displayResult() const {
        // TODO (محمود - Team Leader): Show congratulations message or draw announcement
    }

    void reset() {
        // TODO (محمود - Team Leader): Clear board and reset starting player for new round
        board.reset();
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    Game game;
    game.start();
    return 0;
}