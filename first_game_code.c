#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define BOARD_SIZE 8

// 보드판 상태
typedef enum
{
    EMPTY,
    BLACK,
    WHITE
} CellState;

// 플레이어
typedef enum
{
    PLAYER_BLACK,
    PLAYER_WHITE
} Player;

// 보드판
CellState board[BOARD_SIZE][BOARD_SIZE];

// 현재 플레이어
Player currentPlayer = PLAYER_BLACK;

// 보드판 초기화
void initializeBoard()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = EMPTY;
        }
    }

    int center = BOARD_SIZE / 2;
    board[center - 1][center - 1] = WHITE;
    board[center][center] = WHITE;
    board[center - 1][center] = BLACK;
    board[center][center - 1] = BLACK;
}

// 보드판 그리기
void drawBoard()
{
    clear();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            move(i * 2, j * 4);

            if (board[i][j] == EMPTY)
            {
                printw(".  ");
            }
            else if (board[i][j] == BLACK)
            {
                attron(COLOR_PAIR(1));
                printw("B  ");
                attroff(COLOR_PAIR(1));
            }
            else if (board[i][j] == WHITE)
            {
                attron(COLOR_PAIR(2));
                printw("W  ");
                attroff(COLOR_PAIR(2));
            }
        }
    }

    refresh();
}