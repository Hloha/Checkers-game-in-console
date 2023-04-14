# Game of Checkers
This is a C++ implementation of the classic game of checkers. The game consists of a Board with 64 squares, each of which can contain a Piece. There are two players, Player1 and Player2, who take turns moving their pieces on the board. The game continues until one of the players wins or the game ends in a draw.

# How to Play
1. Run the game by executing the program.
2. The initial board will be displayed with pieces represented by "w" for white pieces, "b" for black pieces, and "-" for empty squares.
3. Enter the coordinates of the piece you want to move in the format "row column". For example, to move a piece in the top left corner of the board, enter "0 0".
4. Enter the coordinates of the square you want to move the piece to in the same format.
5. If the move is valid, the piece will be moved to the new square and any captured pieces will be removed from the board.
6. The game continues until one of the players wins or the game ends in a draw.
7. Follow the on-screen instructions to play the game.

# Game Rules
1. Pieces can only move diagonally and can only move to empty squares.
2. Normal pieces can only move forward, but kings (pieces that have reached the opposite end of the board) can move in any direction.
3. If a piece jumps over an opponent's piece, the opponent's piece is captured and removed from the board.
4. The game ends when one of the players has no more pieces on the board or cannot make a legal move.
5. The player with the most pieces remaining on the board wins. If both players have the same number of pieces, the game ends in a draw.

# Classes
Piece
Represents a piece on the board.
Has a color, which can be white, black, or empty.
Board
Represents the game board.
Contains a vector of shared pointers to pieces.
Has methods to initialize the board, move pieces, print the board, get pieces, and set pieces.
Player
Represents a player in the game.
Has a name and a color, which can be white or black.

# Game
Represents a game of checkers.
Contains a board, player1, player2, and currentPlayer.
Has methods to play the game, check if the game is over, and validate moves.
Note
This implementation does not include support for game rules such as double jumps or kings, and does not handle input validation or error checking. It is a basic implementation that can be extended to add more features and improvements.
