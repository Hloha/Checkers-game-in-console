#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

class Piece {
public:
    enum class Color { WHITE, BLACK, EMPTY };
private:
    Color color;
public:
    Piece(Color color) : color(color) {}

    Color getColor() const {
        return color;
    }

    void setColor(Color color) {
        this->color = color;
    }
};

class Board {
private:
    std::vector<std::shared_ptr<Piece>> board;
    std::vector<std::shared_ptr<Piece>> whitePieces;
    std::vector<std::shared_ptr<Piece>> blackPieces;

    void initializeBoard() {
        for (int i = 0; i < 64; ++i) {
            Piece::Color color = Piece::Color::EMPTY;

            if (i / 8 < 3 && (i + i / 8) % 2 == 1)
                color = Piece::Color::BLACK;
            else if (i / 8 >= 5 && (i + i / 8) % 2 == 1)
                color = Piece::Color::WHITE;

            auto piece = std::make_shared<Piece>(color);
            board.push_back(piece);

            if (color == Piece::Color::WHITE)
                whitePieces.push_back(piece);
            else if (color == Piece::Color::BLACK)
                blackPieces.push_back(piece);
        }
    }
public:
    Board() {
        initializeBoard();
    }

    void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
        auto fromPiece = getPiece(fromRow, fromCol);
        auto toPiece = getPiece(toRow, toCol);

        toPiece->setColor(fromPiece->getColor());
        fromPiece->setColor(Piece::Color::EMPTY);

        if (abs(fromRow - toRow) == 2 && abs(fromCol - toCol) == 2) {
            int capturedRow = (fromRow + toRow) / 2;
            int capturedCol = (fromCol + toCol) / 2;
            auto capturedPiece = getPiece(capturedRow, capturedCol);
            capturedPiece->setColor(Piece::Color::EMPTY);
        }
    }

    void printBoard() {
        std::cout << "  0 1 2 3 4 5 6 7" << std::endl;
        for (int i = 0; i < board.size(); i++) {
            if (i % 8 == 0)
                std::cout << i / 8 << " ";

            char pieceChar = '-';
            if (board[i]->getColor() == Piece::Color::BLACK)
                pieceChar = 'b';
            else if (board[i]->getColor() == Piece::Color::WHITE)
                pieceChar = 'w';

            std::cout << pieceChar << " ";
            if (i % 8 == 7)
                std::cout << std::endl;
        }
    }

    std::shared_ptr<Piece> getPiece(int row, int col) {
        return board[row * 8 + col];
    }

    void setPiece(int row, int col, std::shared_ptr<Piece> piece) {
        board[row * 8 + col] = piece;
    }

    const std::vector<std::shared_ptr<Piece>>& getPieces(Piece::Color color) const {
        if (color == Piece::Color::WHITE)
            return whitePieces;
        else
            return blackPieces;
    }

    friend class Game;
};

class Player {
private:
    std::string name;
    Piece::Color color;
public:
    Player(const std::string& name, Piece::Color color) : name(name), color(color) {}

    const std::string& getName() const {
        return name;
    }

    Piece::Color getColor() const {
        return color;
    }
};

class Game {
private:
    Board board;
    Player player1;
    Player player2;
    Player* currentPlayer;
    std::chrono::time_point<std::chrono::system_clock> startTime;
public:
    Game(const std::string& player1Name, const std::string& player2Name)
        : player1(player1Name, Piece::Color::WHITE),
        player2(player2Name, Piece::Color::BLACK),
        board(),
        currentPlayer(&player1),
        startTime(std::chrono::system_clock::now())
    {}

    void play() {
        while (!isGameOver()) {
            board.printBoard();
            std::cout << currentPlayer->getName() << ", it's your turn!" << std::endl;
            std::cout << "Enter the coordinates of the piece you want to move (row column): ";
            int fromRow, fromCol;
            std::cin >> fromRow >> fromCol;
            std::cout << "Enter the coordinates of the square you want to move the piece to (row column): ";
            int toRow, toCol;
            std::cin >> toRow >> toCol;
            if (isValidMove(fromRow, fromCol, toRow, toCol)) {
                board.movePiece(fromRow, fromCol, toRow, toCol);
            }
            currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
        }
    }

    bool isGameOver() {
        const auto& pieces = board.getPieces(currentPlayer->getColor());
        for (const auto& piece : pieces) {
            for (int newRow = 0; newRow < 8; newRow++) {
                for (int newCol = 0; newCol < 8; newCol++) {
                    int fromRow, fromCol;
                    getPieceCoordinates(piece, fromRow, fromCol);
                    if (isValidMove(fromRow, fromCol, newRow, newCol)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void getPieceCoordinates(const std::shared_ptr<Piece>& piece, int& row, int& col) const {
        for (int i = 0; i < 64; ++i) {
            if (board.board[i] == piece) {
                row = i / 8;
                col = i % 8;
                return;
            }
        }
    }

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) {
        if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
            toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
            return false;
        }

        auto fromPiece = board.getPiece(fromRow, fromCol);
        if (fromPiece->getColor() != currentPlayer->getColor()) {
            return false;
        }

        auto toPiece = board.getPiece(toRow, toCol);
        if (toPiece->getColor() != Piece::Color::EMPTY) {
            return false;
        }

        int direction = (currentPlayer->getColor() == Piece::Color::WHITE) ? -1 : 1;

        if ((fromRow + direction == toRow) && (fromCol - 1 == toCol || fromCol + 1 == toCol)) {
            return true;
        }

        if ((fromRow + 2 * direction == toRow) && (fromCol - 2 == toCol || fromCol + 2 == toCol)) {
            int capturedRow = fromRow + direction;
            int capturedCol = (fromCol + toCol) / 2;
            auto capturedPiece = board.getPiece(capturedRow, capturedCol);
            return capturedPiece->getColor() != currentPlayer->getColor() && capturedPiece->getColor() != Piece::Color::EMPTY;
        }

        return false;
    }
};

int main() {
    Game game("Player 1 ", "Player 2 ");

    game.play();

    return 0;
}