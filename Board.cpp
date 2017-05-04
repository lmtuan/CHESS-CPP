#include "Board.h"
#include <PieceType.h>
#include <stdio.h> // TO DO: remove after debug



Board::Board()
{
  initBoard();
}
Board::~Board() {};

const int Board::dir[8][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0},
                               {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
const int Board::dirKnight[8][2] = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
                                     {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };

void Board::initBoard(){
  player = 1;
  chosenSquare = -1;

  kingSquares[0] = 4;
  kingSquares[1] = 60;
  enPassantCols[0] = -1;
  enPassantCols[1] = -1;

  moveList.clear();
  moveList.reserve(150);
  pinPieces.clear();
  pinPieces.reserve(6);

  // Fill board
  // Fill all squares with -1 (empty)
  for(int i = 16; i < 48; i++)
  {
    board[i/8][i%8] = -1;
  }
  board[0][0] = WR;
  board[0][1] = WN;
  board[0][2] = WB;
  board[0][3] = WQ;
  board[0][4] = WK;
  board[0][5] = WB;
  board[0][6] = WN;
  board[0][7] = WR;

  board[7][0] = BR;
  board[7][1] = BN;
  board[7][2] = BB;
  board[7][3] = BQ;
  board[7][4] = BK;
  board[7][5] = BB;
  board[7][6] = BN;
  board[7][7] = BR;

  for (int j = 0; j < 8; j++)
  {
    board[1][j] = WP;
    board[6][j] = BP;
  }
}

int Board::getPiece(int square)
{
  int piece = board[square/8][square%8];
  if (square == chosenSquare)
  {
    return piece + 12;
  }
  return piece;
}

void Board::printBoard()
{
  printf("Printing Board\n");
  for (int i = 7; i >= 0; i--)
  {
    for (int j = 0; j < 8; j++)
    {
      printf(" %i", board[i][j]);
    }
    printf("\n");
  }
}

void Board::chooseSquare(int square)
{
  // if choose the same square, unchoose the square
  if (square == chosenSquare || square < 0 || square > 63)
  {
    chosenSquare = -1;
  }
  else
  {
    if(chosenSquare == -1) // if currently no square is selected
    {
      int chosenPiece = board[square/8][square%8];
      if ( (chosenPiece >= 0) && ((chosenPiece > 5) == (player == 1)) ) //if the piece chosen is player's piece
      {
        chosenSquare = square;
      }
    }
    else
    {
      makeMove(chosenSquare/8, chosenSquare%8, square/8, square%8);
      chosenSquare = -1;
    }
    printf("Board chosen square %i\n", chosenSquare);
  }
}

void Board::makeMove(int x1,int y1,int x2,int y2)
{
  board[x2][y2] = board[x1][y1];
  board[x1][y1] = -1;
}

void Board::findPinAndCheck()
{
  // Position of King
  int r = kingSquares[player-1] / 8;
  int c = kingSquares[player-1] % 8;
  int i,j;

  // Check 8 basic direction
  for(int d = 0; d <= 8; d++)
  {
    int pinnedSquare = -1;
    i = r;
    j = c;
    while(true)
    {
      // advance 1 square in the direction d
      i += dir[d][0];
      j += dir[d][1];
      // if out of bound
      if (i < 0 || i > 7 || j < 0 || j > 7) break;
      // if empty square, advance 1 more square
      if (board[i][j] == -1) continue;
      if (pinnedSquare == -1) // the first obstacle
      {
        if ( (player == 1) == (board[i][j] > 5) ) // if it's current player's piece
        {
          pinnedSquare = i * 8 + j;
        }
        else // if it's opponent's piece
        {
          // Check if it's checking current king
          int curPiece = board[i][j];
          if (curPiece < 6) curPiece += 6; // turn into white for easy switch
          if (curPiece == WQ || (d < 4 && curPiece == WR) || (d > 3 && curPiece == WB))
          {
            int checkIndex = (checkingPieces[0] == -1)? 0 : 1;
            checkingPieces[checkIndex] = i * 8 + j;
          }
          break;
        }
      }
      else // the second obstacle
      {
        if ( (player == 1) != (board[i][j] > 5) ) // if it's opponent's piece
        {
          int curPiece = board[i][j];
          if (curPiece < 6) curPiece += 6; // turn into white for easy switch
          if (curPiece == WQ || (d < 4 && curPiece == WR) || (d > 3 && curPiece == WB)) // if it's opponent's ray piece
          {
            pinPieces.push_back(pinnedSquare);
            pinPieces.push_back(i* 8 + j);
          }
        }
        break;
      }
    }
  }

  if (checkingPieces[1] != -1) return;

  // Check 8 knight's move
  for (int d = 0; d < 8; d++)
  {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i > 7 || j < 0 || j > 7) continue;

    if (board[i][j] == (player == 1? BN : WN) ) // if has opponent's knight
    {
      int t = (checkingPieces[0] == -1)? 0 : 1;
      checkingPieces[t] = i * 8 + j;
      // if there are 2 checking pieces, that's max
      if (t == 1) return;
    }
  }

  // Check pawn's move
  i = (player == 1)? (r + 1) : (r - 1);
  j = c - 1; // The left column
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if ( board[i][j] == (player == 1? BP:WP) )
    {
      int t = (checkingPieces[0] == -1)? 0 : 1;
      checkingPieces[t] = i * 8 + j;
      if (t == 1) return;
    }
  }
  j = c + 1; // The right column
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if (board[i][j] == (player == 1? BP:WP))
    {
      int t = (checkingPieces[0] == -1)? 0 : 1;
      checkingPieces[t] = i * 8 + j;
    }
  }
}

bool Board::isSquareControlled(int square)
{
  int r = square / 8;
  int c = square % 8;
  int i,j;

  /*
   * Check 8 knight's move
   */
  for (int d = 0; d < 8; d++)
  {
    i = r + dirKnight[d][0];
    j = c + dirKnight[d][1];
    // if out of bound, check other directions
    if ( i < 0 || i > 7 || j < 0 || j > 7) continue;
    if (board[i][j] == (player == 1? BN : WN) ) return true;
  }

  /*
   * Check 8 ray directions
   */
  for(int d = 0; d <= 8; d++)
  {
    i = r;
    j = c;
    while(true)
    {
      // advance 1 square in the direction d
      i += dir[d][0];
      j += dir[d][1];
      // if out of bound
      if (i < 0 || i > 7 || j < 0 || j > 7) break;
      // if empty square, advance 1 more square
      if (board[i][j] == -1) continue;
      // if it is opponent's ray piece
      if ( (player == 1) != (board[i][j] > 5) )
      {
        int curPiece = board[i][j];
        if (curPiece < 6) curPiece += 6; //turn into whiteint curPiece = board[i][j];
        if (curPiece == WQ || (d < 4 && curPiece == WR) || (d > 3 && curPiece == WB)) return true;
      }
      break;
    }
  }
  // Check pawn's move
  i = (player == 1)? (r + 1) : (r - 1);
  j = c - 1; // left column
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if (board[i][j] == (player == 1? BP : WP)) return true;
  }
  j = c + 1;
  if (i >= 0 && i < 8 && j >= 0 && j < 8)
  {
    if (board[i][j] == (player == 1? BP : WP)) return true;
  }

  // if pass all tests, square is not controlled
  return false;
}

bool Board::isInRay(int x1, int y1, int x2, int y2, int x3, int y3)
{
  if(x1 == x3) // same row
  {
    if (x2 == x3 && ((y2 > y1 && y2 < y3) || (y2 < y1 && y2 > y3)) ) return true;//if in line and square 2 is in the middle
  }
  else if (y1 == y3) // same col
  {
    if (y2 == y3 && ((x2 > x1 && x2 < x3) || (x2 < x1 && x2 > x3)) ) return true;//if in line and square 2 is in the middle
  }
  else if ( (x2 - x1)*(y3-y1) == (y2 - y1)*(x3-x1) ) // same diagonal
  {
    if ((x2 > x1 && x2 < x3) || (x2 < x1 && x2 > x3)) return true;
  }
  return false;
}
