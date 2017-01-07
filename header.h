#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <ctime>
#include <iostream>
#define MAX 100

/*
0 is empty
1-8 is number
9 is mine
10 is unrevealed square
11 is flag
*/

// Definitions
int width = 5, height = 5, squareNumber, mineNumber, unrevealed, flagNum, minesLeft, faceSize, gameTime = 0, menustage = 0;
int squareSize = 32;
int gameBorder_top = 4, gameBorder_right = 1, gameBorder_bottom = 1, gameBorder_left = 1;
int board[MAX*MAX] = {0};
int boardRevealed[MAX*MAX] = {0}; // -2 is point of start, -1 is point of dead, 0 is unrevealed, 1 is revealed, 2 is flagged, 3 is not sure
bool first_click = true, gameOver = false, won = false, music_sound = true, sfx_sound = true, mark_flag = true, display_size = true;

// Funtions meta:
int nearMines(int x, int y);
void countSurrounding(int x, int y);
void placeMines(int first_click_poz);
bool revealed (int x, int y);
void display(int x, int y, int firstX, int firstY);
bool reveal(int x, int y);
void reset();
bool show_surroundings(int x, int y);

// All Functions here:
int nearMines(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y <= height)
    {
        if (board[y * width + x] == 9) return 1;
        else return 0;
    }
    else return 0;
}

void countSurrounding(int x, int y)
{
    if (board[y * width + x] == 9) return;
    unsigned int result = 0;
    if (nearMines(x + 1, y)) result++;
    if (nearMines(x - 1, y)) result++;
    if (nearMines(x, y - 1)) result++;
    if (nearMines(x, y + 1)) result++;
    if (nearMines(x + 1, y + 1)) result++;
    if (nearMines(x + 1, y - 1)) result++;
    if (nearMines(x - 1, y + 1)) result++;
    if (nearMines(x - 1, y - 1)) result++;
    board[y * width + x] = result;
}

void placeMines(int first_click_poz)
{
    int count = 0;
    while (count < mineNumber)
    {
        int randPosition = rand() % squareNumber;
        if (board[randPosition] != 9 && board[randPosition]!=-2)
        {
            board[randPosition] = 9;
            ++count;
        }
    }
    if (board[first_click_poz]==-2)
        board[first_click_poz]=0;
}

bool revealed(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return true;
    if (boardRevealed[y * width + x] == 1 || boardRevealed[y * width + x] == 2) return true;
    return false;
}

void display(int x, int y, int firstX, int firstY)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    int i = y * width + x;
    if (boardRevealed[i] == 1 || boardRevealed[i] == 2 || board[i] == 9) return;
    boardRevealed[i] = 1;
    --unrevealed;
    if (board[i] == 0)
    {
        if (!revealed(x + 1, y))
        {
            display(x + 1, y, x, y);
        }
        if (!revealed(x - 1, y))
        {
            display(x - 1, y, x, y);
        }
        if (!revealed(x, y + 1))
        {
            display(x, y + 1, x, y);
        }
        if (!revealed(x, y - 1))
        {
            display(x, y - 1, x, y);
        }
        if (!revealed(x + 1, y + 1))
        {
            display(x + 1, y + 1, x, y);
        }
        if (!revealed(x + 1, y - 1))
        {
            display(x + 1, y - 1, x, y);
        }
        if (!revealed(x - 1, y + 1))
        {
            display(x - 1, y + 1, x, y);
        }
        if (!revealed(x - 1, y - 1))
        {
            display(x - 1, y - 1, x, y);
        }
    }
}

bool reveal(int x, int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    int i = y * width + x;
    if (boardRevealed[i] == 0 || boardRevealed[i] == 3)
    {
        boardRevealed[i] = 1;
        --unrevealed;
        if (board[i] == 0)
        {
            display(x + 1, y, x, y);
            display(x - 1, y, x, y);
            display(x, y + 1, x, y);
            display(x, y - 1, x, y);
            display(x + 1, y - 1, x, y);
            display(x + 1, y + 1, x, y);
            display(x - 1, y - 1, x, y);
            display(x - 1, y + 1, x, y);
        }
        if (board[i] == 9)
        {
            gameOver = true;
            return true;
        }
    }
    return false;
}

void reset()
{
    for(int i=0; i<squareNumber; i++)
    {
        board[i]=0;
        boardRevealed[i]=0;
    }
    gameOver = false;
    first_click = true;
    won = false;
    minesLeft = mineNumber;
    unrevealed = squareNumber;
    gameTime = 0;
}

bool show_surroundings(int x, int y)
{
    int flag=0;
    if(boardRevealed[(y-1)*width+x]==2) flag++;
    if(boardRevealed[(y-1)*width+(x-1)]==2) flag++;
    if(boardRevealed[(y-1)*width+(x+1)]==2) flag++;
    if(boardRevealed[y*width+(x-1)]==2) flag++;
    if(boardRevealed[y*width+(x+1)]==2) flag++;
    if(boardRevealed[(y+1)*width+x]==2) flag++;
    if(boardRevealed[(y+1)*width+(x-1)]==2) flag++;
    if(boardRevealed[(y+1)*width+(x+1)]==2) flag++;
    if(flag==board[y*width+x])
    {
        if( reveal(x,y-1)   ||
                reveal(x-1,y-1) ||
                reveal(x+1,y-1) ||
                reveal(x-1,y)   ||
                reveal(x+1,y)   ||
                reveal(x,y+1)   ||
                reveal(x-1,y+1) ||
                reveal(x+1,y+1))
            return true;
    }
    return false;
}
