#ifndef SERVER_H
#define SERVER_H

#define BOARD_SIZE 8 

// 서버 관련 헤더 파일 인클루드
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
// #include <linux/socket.h>

void waitForClient(char * portNumber) 

void receiveMessageFromClient(int connfd) ; 

void sendMessageToClient(int connfd, char* message) ;

void displayBoard() ;

void playGame() ;

#endif /* SERVER_H */