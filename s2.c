#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 9000
#define BUF_SIZE 1024

int calculate(int num1, int num2, char op) {
    int result;
    switch(op) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            result = num1 / num2;
            break;
        default:
            printf("Invalid operator\n");
            exit(EXIT_FAILURE);
    }
    return result;
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    char buf[BUF_SIZE];
    int num1, num2, result;
    char op;

    // Create a socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_sock, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connections and process requests
    printf("Server started, waiting for incoming connections...\n");
    while (1) {
        socklen_t client_len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Receive request from client
        memset(&buf, 0, sizeof(buf));
        int n = read(client_sock, buf, BUF_SIZE);
        if (n < 0) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }

        sscanf(buf, "%d %c %d", &num1, &op, &num2);

        // Calculate result
        result = calculate(num1, num2, op);

        // Send response to client
        memset(&buf, 0, sizeof(buf));
        sprintf(buf, "Result is: %d\n", result);
        n = write(client_sock, buf, strlen(buf));
        if (n < 0) {
            perror("write failed");
            exit(EXIT_FAILURE);
        }

        printf("Result sent to client %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Close client socket
        close(client_sock);
    }

    // Close server socket
    close(server_sock);

    return 0;
}
