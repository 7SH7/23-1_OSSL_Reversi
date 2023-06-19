#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 12345

void drawBoard(const char* board) {
    printf("   1 2 3 4 5 6 7 8\n");
    printf("  -----------------\n");
    for (int i = 0; i < 8; i++) {
        printf("%c |", 'A' + i);
        for (int j = 0; j < 8; j++) {
            printf("%c|", board[i * 8 + j]);
        }
        printf("\n");
        printf("  -----------------\n");
    }
}

int main() {
    struct sockaddr_in serverAddress;
    char board[64];
    char buffer[10];
    int row, col;
    int currentPlayer = 1;  // 1: Player 1 (X), 2: Player 2 (O)

    // 클라이언트 소켓 생성
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serverAddress.sin_port = htons(SERVER_PORT);

    // 서버에 연결
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server!\n");

    while (1) {
        // 보드판 상태 수신
        if (read(clientSocket, board, sizeof(board)) <= 0) {
            perror("Failed to receive board state");
            break;
        }

        // 보드판 출력
        drawBoard(board);

        // 현재 플레이어 출력
        printf("Current Player: Player %d (%c)\n", currentPlayer, (currentPlayer == 1) ? 'X' : 'O');

        // 위치 입력 받기
        printf("Enter move (e.g., A1): ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%c%d", &buffer[0], &buffer[1]);
        row = buffer[0] - 'A';
        col = buffer[1] - '1';

        // 위치 전송
        write(clientSocket, &row, sizeof(row));
        write(clientSocket, &col, sizeof(col));

        // 게임 종료 조건 확인
        if (read(clientSocket, &row, sizeof(row)) <= 0) {
            perror("Failed to receive game result");
            break;
        }

        if (row == -1) {
            printf("Invalid move! Try again.\n");
        } else if (row == 0) {
            printf("Game Over! It's a draw.\n");
            break;
        } else {
            printf("Player %d (%c) wins!\n", row, (row == 1) ? 'X' : 'O');
            break;
        }

        // 플레이어 변경
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    // 소켓 닫기
    close(clientSocket);

    return 0;
}