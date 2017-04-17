#include "BoardGUI.h"
#include <TextureWrapper.h>
//#include <PieceType.h>

#include <stdio.h>

BoardGUI::BoardGUI()
{
  Box* boxArr = new Box[64];
  for(int i = 0; i < 64; i++) {
    int x = 54 + (i%8) * 590 / 8;
    int y = 55 + (7- i/8) * 590 / 8;
    boxArr[i] = {x, y , x+73, y+73, i+1};
  }
  setBoxes(boxArr, 64);
}

void BoardGUI::initGUI(SDL_Renderer* renderer)
{
  bool loadBoard = boardImg.loadFromFile(renderer, "img/board.jpg");
  //if (!loadBoard) printf("null chessboared");
  bool loadPiece = piecesSprite.loadFromFile(renderer, "img/pieces.png");
  //if(!loadPiece) printf("null piece");

  //position of piece's image on sprite sheet
  for(int i = 0; i < 12; i++) {
    pieceClips[i] = { (i%6)*60, (i/6)*60, 60, 60 };
  }
  // positions of squares in chessboard
  for(int i = 0; i < 64; i++) {
    int x = 54 + (i%8) * 590 / 8;
    int y = 55 + (7 - i/8) * 590 / 8;
    boardSquares[i] = {x, y ,73, 73};
  }

}

void BoardGUI::draw(SDL_Renderer* renderer, Board* brd)
{
  // Clear screen
  SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  SDL_RenderClear( renderer );

  boardImg.render(renderer, 0, 0, NULL, NULL);

  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
  SDL_RenderDrawRects(renderer, boardSquares, 64);

  for (int i = 0; i < 64; i++)
  {
    int piece = brd->getPiece(i);
    if (piece >= 0)
    {
      piecesSprite.render(renderer, &pieceClips[piece], &boardSquares[i]);
    }
  }
  SDL_RenderPresent(renderer);
}