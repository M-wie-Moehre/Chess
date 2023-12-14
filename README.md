# Chess
Little fully functional chess game build with SFML.

All extra rules, like **en passant** and **castling**, as well as a very basic menu are included.
You can also play online on different PCs, by entering the other persons ip address.

<div align="center">
  <img src="/textures/chess_1.png" width="45%" padding-right="20px">
  <img src="/textures/chess_2.png" width="45%">
</div>

## Things to add in the future:
- flipping the board after every move

## Small explanation of the code:
[chess.cpp](chess.cpp) is the main file, in which the header files get included. Every `mode` (for example [menu](menu.h), [game](game.h) etc...) has its own header file. They include the logic and draw functions. The [game.h](game.h) file contains the logic of the chess game itself.

### The chess code itself:
It works roughly the following way: When a piece is picked up, the game updates the list `validPiecePositions`. It contains all the positions the piece can move to. That way we don't have to check it every time again. It updates the list with the `updateValidPiecePositions` function. It calls the `canPieceActuallyMoveHere` function for every field, which checks if the current piece can move to the given position (with checking if the king would be in check). To determine if the piece can move to the given position in the first place, the `canPieceActuallyMoveHere` function calls the `canPieceMoveHere` function, which checks the basic rules (like not beeing able to put a piece on the same color), the piece specific rules (like that the rook can only move vertical and horizontal) and also the *special* rules (like castling and es passant), however it doesn't check if the king would be in check.

The same way we also determine if one player is checkmate or the game ends in stalemate. Both the `isCheckmate` and the `isStalemate` function go trough every piece of the own color and check if it can save the king (for checkmate) or still move (for stalemate). It does it the same way as the already mentioned function *updateValidPiecePositions*.

### The code to play online:
When playing together, the game will connect to the other PC by using TCP-Sockets, which are also build into SFML. You can either choose to create a game, which will make you the host, or to join a game, which will make you the client. When creating a game, the game will create a TCP-Listener, which will listen on port 55001 for an incoming connection from the client. If one comes in, the listener will create a connection between the to PCs (with the TCP-Sockets). On the other hand, if you want to join a game, the game will use the TCP-Socket to try to connect to the (entered) IP-Address of the host on port 55001. When after one second the TCP-Socket couldn't connect to the host, simply press the join game button again.
