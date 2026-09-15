#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

enum Difficulty {
    EASY,
    HARD
};

class Board {
private:
    vector<vector<char>> grid;
    const int size;

public:
    Board(int size = 3) : size(size) {
        grid.resize(size, vector<char>(size, ' '));
    }

    void display() const {
        cout << "\n    1   2   3\n";
        for (int i = 0; i < size; ++i) {
            cout << " " << (i + 1) << "  ";
            for (int j = 0; j < size; ++j) {
                cout << grid[i][j];
                if (j < size - 1) cout << " | ";
            }
            cout << "\n";
            if (i < size - 1) {
                cout << "   ---+---+---\n";
            }
        }
        cout << "\n";
    }

    bool isValidMove(int row, int col) const {
        return (row >= 0 && row < size && col >= 0 && col < size && grid[row][col] == ' ');
    }

    bool makeMove(int row, int col, char symbol) {
        if (!isValidMove(row, col)) {
            return false;
        }
        grid[row][col] = symbol;
        return true;
    }

    void undoMove(int row, int col) {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            grid[row][col] = ' ';
        }
    }

    bool checkWin(char symbol) const {
        for (int i = 0; i < size; ++i) {
            if (grid[i][0] == symbol && grid[i][1] == symbol && grid[i][2] == symbol) return true;
            if (grid[0][i] == symbol && grid[1][i] == symbol && grid[2][i] == symbol) return true;
        }
        if (grid[0][0] == symbol && grid[1][1] == symbol && grid[2][2] == symbol) return true;
        if (grid[0][2] == symbol && grid[1][1] == symbol && grid[2][0] == symbol) return true;
        return false;
    }

    bool isFull() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (grid[i][j] == ' ') return false;
            }
        }
        return true;
    }

    char getCell(int row, int col) const {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            return grid[row][col];
        }
        return ' ';
    }

    void reset() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                grid[i][j] = ' ';
            }
        }
    }

    int getSize() const {
        return size;
    }
};

class Player {
protected:
    string name;
    char symbol;

public:
    Player(const string& name, char symbol) : name(name), symbol(symbol) {}
    virtual ~Player() = default;

    virtual void getMove(int& row, int& col) = 0;

    string getName() const { return name; }
    char getSymbol() const { return symbol; }
    void setName(const string& newName) { name = newName; }
};

class HumanPlayer : public Player {
public:
    HumanPlayer(const string& name, char symbol) : Player(name, symbol) {}

    void getMove(int& row, int& col) override {}
};

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

    void getMove(int& row, int& col) override {}

    void getAIMove(Board& board, int& row, int& col) {
        if (difficulty == EASY) {
            getRandomMove(board, row, col);
        } else {
            getBestMove(board, row, col);
        }
    }

    void getRandomMove(const Board& board, int& row, int& col) const {
        vector<pair<int, int>> available;
        for (int i = 0; i < board.getSize(); ++i) {
            for (int j = 0; j < board.getSize(); ++j) {
                if (board.isValidMove(i, j)) {
                    available.push_back({i, j});
                }
            }
        }
        if (!available.empty()) {
            int idx = rand() % available.size();
            row = available[idx].first;
            col = available[idx].second;
        }
    }

    int evaluateBoard(const Board& board) const {
        char opponentSymbol = (symbol == 'X') ? 'O' : 'X';
        if (board.checkWin(symbol)) return +10;
        if (board.checkWin(opponentSymbol)) return -10;
        return 0;
    }

    int minimax(Board& board, int depth, bool isMaximizing) {
        int score = evaluateBoard(board);
        if (score == 10) return score - depth;
        if (score == -10) return score + depth;
        if (board.isFull()) return 0;

        char opponentSymbol = (symbol == 'X') ? 'O' : 'X';

        if (isMaximizing) {
            int best = -1000;
            for (int i = 0; i < board.getSize(); ++i) {
                for (int j = 0; j < board.getSize(); ++j) {
                    if (board.isValidMove(i, j)) {
                        board.makeMove(i, j, symbol);
                        best = max(best, minimax(board, depth + 1, false));
                        board.undoMove(i, j);
                    }
                }
            }
            return best;
        } else {
            int best = 1000;
            for (int i = 0; i < board.getSize(); ++i) {
                for (int j = 0; j < board.getSize(); ++j) {
                    if (board.isValidMove(i, j)) {
                        board.makeMove(i, j, opponentSymbol);
                        best = min(best, minimax(board, depth + 1, true));
                        board.undoMove(i, j);
                    }
                }
            }
            return best;
        }
    }

    void getBestMove(Board& board, int& row, int& col) {
        int bestVal = -1000;
        row = -1;
        col = -1;

        for (int i = 0; i < board.getSize(); ++i) {
            for (int j = 0; j < board.getSize(); ++j) {
                if (board.isValidMove(i, j)) {
                    board.makeMove(i, j, symbol);
                    int moveVal = minimax(board, 0, false);
                    board.undoMove(i, j);

                    if (moveVal > bestVal) {
                        bestVal = moveVal;
                        row = i;
                        col = j;
                    }
                }
            }
        }
    }
};

class Game {
private:
    Board board;
    Player* player1;
    Player* player2;
    Player* currentPlayer;

public:
    Game() : board(3), player1(nullptr), player2(nullptr), currentPlayer(nullptr) {}

    ~Game() {
        cleanPlayers();
    }

    void cleanPlayers() {
        delete player1;
        delete player2;
        player1 = nullptr;
        player2 = nullptr;
        currentPlayer = nullptr;
    }

    void showMenu() {
        cout << "=================================\n";
        cout << "       TIC-TAC-TOE GAME          \n";
        cout << "=================================\n";
        cout << "1. Player vs Player\n";
        cout << "2. Player vs Computer (Easy)\n";
        cout << "3. Player vs Computer (Hard)\n";
        cout << "4. Exit\n";
        cout << "Select game mode: ";
    }

    void setupPvP() {
        cleanPlayers();
        string name1, name2;
        cout << "Enter Player 1 name (X): ";
        cin >> name1;
        cout << "Enter Player 2 name (O): ";
        cin >> name2;
        player1 = new HumanPlayer(name1, 'X');
        player2 = new HumanPlayer(name2, 'O');
        currentPlayer = player1;
    }

    void setupPvC(Difficulty difficulty) {
        cleanPlayers();
        string name;
        cout << "Enter your name (X): ";
        cin >> name;
        player1 = new HumanPlayer(name, 'X');
        player2 = new AIPlayer("Computer", 'O', difficulty);
        currentPlayer = player1;
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }

    void handleHumanMove(Player* player) {
        int r, c;
        while (true) {
            cout << player->getName() << " (" << player->getSymbol() << "), enter your move (row and column: 1-3): ";
            if (cin >> r >> c) {
                int rowIdx = r - 1;
                int colIdx = c - 1;
                if (board.isValidMove(rowIdx, colIdx)) {
                    board.makeMove(rowIdx, colIdx, player->getSymbol());
                    break;
                } else {
                    cout << "Invalid move. Cell is either occupied or out of bounds. Try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter two integers between 1 and 3.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    void handleAIMove(AIPlayer* aiPlayer) {
        cout << aiPlayer->getName() << " (" << aiPlayer->getSymbol() << ") is thinking...\n";
        int r = -1, c = -1;
        aiPlayer->getAIMove(board, r, c);
        board.makeMove(r, c, aiPlayer->getSymbol());
        cout << aiPlayer->getName() << " placed '" << aiPlayer->getSymbol() << "' at " << (r + 1) << " " << (c + 1) << "\n";
    }

    bool checkGameOver() {
        if (board.checkWin(currentPlayer->getSymbol())) {
            board.display();
            cout << "Congratulations! " << currentPlayer->getName() << " wins!\n";
            return true;
        }
        if (board.isFull()) {
            board.display();
            cout << "The game ended in a draw!\n";
            return true;
        }
        return false;
    }

    void reset() {
        board.reset();
        currentPlayer = player1;
    }

    void playRound() {
        reset();
        while (true) {
            board.display();
            if (HumanPlayer* hp = dynamic_cast<HumanPlayer*>(currentPlayer)) {
                handleHumanMove(hp);
            } else if (AIPlayer* ap = dynamic_cast<AIPlayer*>(currentPlayer)) {
                handleAIMove(ap);
            }

            if (checkGameOver()) {
                break;
            }

            switchPlayer();
        }
    }

    void start() {
        int choice;
        while (true) {
            showMenu();
            if (!(cin >> choice)) {
                cout << "Invalid selection. Please enter a valid number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (choice == 4) {
                cout << "Exiting game. Goodbye!\n";
                break;
            }

            if (choice == 1) {
                setupPvP();
            } else if (choice == 2) {
                setupPvC(EASY);
            } else if (choice == 3) {
                setupPvC(HARD);
            } else {
                cout << "Choice out of range. Please choose between 1 and 4.\n";
                continue;
            }

            char replay = 'y';
            while (replay == 'y' || replay == 'Y') {
                playRound();
                cout << "Do you want to play again? (y/n): ";
                cin >> replay;
            }
        }
    }
};

int main() {
    Game game;
    game.start();
    return 0;
}
