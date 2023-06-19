#include "server.h"

int connfd;
int board[8][8];

void waitForClient(char *portNumber)
{
    // 클라이언트의 연결을 기다리는 함수
    int listenfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    int portnum = atoi(portNumber);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portnum);
    // 클라이언트 연결 완료
}

void receiveMessageFromClient()
{
    // 클라이언트로부터 메시지를 받아와 처리하는 함수
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    recv(connfd, buffer, sizeof(buffer), 0);

    // 받은 메시지 처리
    // 예시로 받은 좌표를 게임 로직에 적용하여 보드 업데이트
    int row = buffer[1] - '0';
    int col = buffer[0] - 'A';

    // 보드 업데이트 로직 추가

    // 클라이언트에게 업데이트된 보드 상태 전송
    sendMessageToClient(buffer);
}

void sendMessageToClient(char *message)
{
    // 클라이언트로 메시지를 보내는 함수
    send(connfd, message, strlen(message), 0);
}

void displayBoard()
{
    // 보드 상태를 표시하는 함수
    // 예시로 간단한 텍스트 기반의 보드 표시
    printf("   A B C D E F G H\n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void playGame()
{
    // 게임 로직을 실행하는 함수
    while (1)
    {
        displayBoard();
        receiveMessageFromClient();

        // 게임 종료 조건 체크
        // 예시로 특정 조건을 만족하면 "GAME OVER" 메시지 전송
        if (isGameOver())
        {
            sendMessageToClient("GAME OVER");
            break;
        }
    }
}