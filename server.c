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

// 돌 뒤집기 + 놓을 자리 없을 경우 다음 턴으로 넘기기
void flipPieces(int row, int col, int dRow, int dCol)
{
    CellState currentColor = (currentPlayer == PLAYER_BLACK) ? BLACK : WHITE;
    CellState opponentColor = (currentPlayer == PLAYER_BLACK) ? WHITE : BLACK;

    int newRow = row + dRow;
    int newCol = col + dCol;

    while (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)
    {
        if (board[newRow][newCol] == opponentColor)
        {
            newRow += dRow;
            newCol += dCol;
        }
        else if (board[newRow][newCol] == currentColor)
        {
            while (newRow != row || newCol != col)
            {
                newRow -= dRow;
                newCol -= dCol;
                board[newRow][newCol] = currentColor;
            }
            return;
        }
        else if (board[newRow][newCol] == EMPTY)
        {
            return;
        }
    }
}

// 돌을 놓을 수 있는지 확인하는 함수
int isMoveValid(int row, int col)
{
    if (board[row][col] != EMPTY)
    {
        return 0; // 이미 돌이 있는 위치이므로 유효하지 않음
    }

    CellState playerColor = (currentPlayer == PLAYER_BLACK) ? BLACK : WHITE;
    CellState opponentColor = (currentPlayer == PLAYER_BLACK) ? WHITE : BLACK;

    // 주변 8방향에 상대방 돌이 있는지 확인
    for (int dRow = -1; dRow <= 1; dRow++)
    {
        for (int dCol = -1; dCol <= 1; dCol++)
        {
            if (dRow == 0 && dCol == 0)
            {
                continue;
            }

            int newRow = row + dRow;
            int newCol = col + dCol;
            int foundOpponentPiece = 0;

            while (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)
            {
                if (board[newRow][newCol] == opponentColor)
                {
                    foundOpponentPiece = 1;
                    newRow += dRow;
                    newCol += dCol;
                }
                else if (board[newRow][newCol] == playerColor)
                {
                    if (foundOpponentPiece)
                    {
                        return 1; // 주변에 상대방 돌이 있고, 돌을 뒤집을 돌도 있으므로 유효함
                    }
                    else
                    {
                        break;
                    }
                }
                else if (board[newRow][newCol] == EMPTY)
                {
                    break; // 빈 공간이므로 유효하지 않음
                }
            }
        }
    }

    return 0; // 주변에 상대방 돌이 없어 뒤집을 돌이 없으므로 유효하지 않음
}

// 돌 놓기
void placePiece(int row, int col)
{

    if (!isMoveValid(row, col))
    {
        return; // 돌을 놓을 수 없는 경우 함수 종료
    }

    CellState playerColor = (currentPlayer == PLAYER_BLACK) ? BLACK : WHITE;
    board[row][col] = playerColor;

    for (int dRow = -1; dRow <= 1; dRow++)
    {
        for (int dCol = -1; dCol <= 1; dCol++)
        {
            if (dRow == 0 && dCol == 0)
            {
                continue;
            }

            flipPieces(row, col, dRow, dCol);
        }
    }

    currentPlayer = (currentPlayer == PLAYER_BLACK) ? PLAYER_WHITE : PLAYER_BLACK;

    // 상대방 플레이어가 돌을 놓을 수 있는지 확인
    int opponentHasMove = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (isMoveValid(i, j))
            {
                opponentHasMove = 1;
                break;
            }
        }
        if (opponentHasMove)
        {
            break;
        }
    }

    // 상대방 플레이어에게 턴을 넘김
    if (!opponentHasMove)
    {
        currentPlayer = (currentPlayer == PLAYER_BLACK) ? PLAYER_WHITE : PLAYER_BLACK;
    }
}

// 게임 종료 여부 체크
int isGameOver()
{
    int blackCount = 0;
    int whiteCount = 0;
    int emptyCount = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == BLACK)
            {
                blackCount++;
            }
            else if (board[i][j] == WHITE)
            {
                whiteCount++;
            }
            else if (board[i][j] == EMPTY)
            {
                emptyCount++;
            }
        }
    }

    if (blackCount == 0 || whiteCount == 0 || emptyCount == 0)
    {
        return 1;
    }

    return 0;
}

// 게임 종료 후 결과 출력
void printGameResult()
{
    int blackCount = 0;
    int whiteCount = 0;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == BLACK)
            {
                blackCount++;
            }
            else if (board[i][j] == WHITE)
            {
                whiteCount++;
            }
        }
    }

    if (blackCount > whiteCount)
    {
        printw("Black wins!\n");
    }
    else if (blackCount < whiteCount)
    {
        printw("White wins!\n");
    }
    else
    {
        printw("It's a tie!\n");
    }
}
