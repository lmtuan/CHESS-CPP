#ifndef BOARDGUI_H
#define BOARDGUI_H

#include <GUI.h>
#include <TextureWrapper.h>
#include <Board.h>


class BoardGUI : public GUI
{
  public:
    /**
     * Create an array of box and pass it to parent class GUI through GUI.setBoxes()
     * Load images to use in game (in the form of TextureWrapper)
     * @param brd the pointer to the drawn board
     * @param renderer the renderer of the window to be drawn on
     */
    BoardGUI(Board* brd, SDL_Renderer* renderer);

    /**
     * draw the chessboard screen
     * @param renderer the renderer of the window to be drawn on
     */
    void draw(SDL_Renderer* renderer);

  private:
    /**
     * The TextureWrapper contains images used in display
     * Background image, image of chessboard, image of chess pieces
     */
    Board* b;
    TextureWrapper bgImg, boardImg, piecesSprite;
    TextureWrapper undoButton, undoButtonFaded, undoButtonGlow;
    TextureWrapper homeButton, homeButtonGlow;
    TextureWrapper playerTxt, comTxt, promoteTxt;
    TextureWrapper moveArrow;
    TextureWrapper colorSymbols[2];
    const static Uint8 ALPHA_FADED = 100;
    const static Uint8 ALPHA_NORMAL = 255;




    /**
     * Coordinates of images of pieces on the sprite sheet
     */
    SDL_Rect pieceClips[12];
    /**
     *  Coordinate of 64 squares of chessboard
     */
    SDL_Rect boardSquares[64];

    SDL_Rect undoRect, homeRect, playerTxtRect, colorSymbolRect, promoteTxtRect;
    SDL_Rect promotePieceRects[4];
};
#endif // BOARDGUI_H
