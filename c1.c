#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main()
{
    struct sockaddr_in servaddr = {0};
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("failed to create socket");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(12345);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    char input[50] = {0};
    printf("Enter two numbers and an operation (+, -, *, /) separated by spaces:\n");
    fgets(input, 50, stdin);

    int len = sendto(sockfd, (const char *)input, strlen(input), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    if (len == -1)
    {
        perror("failed to send");
    }

    char result[50] = {0};
    len = recvfrom(sockfd, (char *)result, 50, 0, NULL, NULL);
    if (len == -1)
    {
        perror("failed to receive");
    }
    printf("Result: %s\n", result);

    close(sockfd);

    return 0;
}


