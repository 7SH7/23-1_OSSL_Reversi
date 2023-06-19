#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 9000

// ... (기존에 제공된 오셀로 게임 관련 함수 및 변수들)

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength;

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server details
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(SERVER_PORT);

    // Bind socket to address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(serverSocket, 1) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a client to connect...\n");

    // Accept a client connection
    clientAddressLength = sizeof(clientAddress);
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength)) < 0) {
        perror("Acceptance failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected!\n");

    // Send the initial game state to the client
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s", getBoardAsString());  // 기존에 제공된 함수
    if (send(clientSocket, buffer, strlen(buffer), 0) < 0) {
        perror("Failed to send data");
        exit(EXIT_FAILURE);
    }

    // Game loop
    while (1) {
        // Receive the user's move from the client
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead < 0) {
            perror("Failed to receive data");
            exit(EXIT_FAILURE);
        }

        // Update the game state with the user's move
        processUserMove(buffer);  // 기존에 제공된 함수

        // Send the updated game state to the client
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "%s", getBoardAsString());  // 기존에 제공된 함수
        if (send(clientSocket, buffer, strlen(buffer), 0) < 0) {
            perror("Failed to send data");
            exit(EXIT_FAILURE);
        }
    }

    // Close the sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
