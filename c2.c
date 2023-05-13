#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000
#define BUF_SIZE 1024

int main() {
    int client_sock;
    struct sockaddr_in server_addr;
    char buf[BUF_SIZE];
    int num1, num2;
    char op;

    // Create a socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("invalid server address");
        exit(EXIT_FAILURE);
    }
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    // Get input from user
    printf("Enter first number: ");
    scanf("%d", &num1);
    printf("Enter second number: ");
    scanf("%d", &num2);
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &op);

    // Send request to server
    memset(&buf, 0, sizeof(buf));
    sprintf(buf, "%d %c %d", num1, op, num2);
    int n = write(client_sock, buf, strlen(buf));
    if (n < 0) {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    // Receive response from server
    memset(&buf, 0, sizeof(buf));
    n = read(client_sock, buf, BUF_SIZE);
    if (n < 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }

    printf("%s", buf);

    // Close socket
    close(client_sock);

    return 0;
}
