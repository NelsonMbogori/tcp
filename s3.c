#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <netinet/in.h>


#define PORT 12345


int main()

{

    struct sockaddr_in servaddr = {0};


    // Create socket

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)

    {

        perror("failed to create socket");

        exit(EXIT_FAILURE);

    }


    // Bind socket to address and port

    servaddr.sin_family = AF_INET;

    servaddr.sin_addr.s_addr = INADDR_ANY;

    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)

    {

        perror("failed to bind socket");

        exit(EXIT_FAILURE);

    }


    printf("Server listening on port %d\n", PORT);


    while(1) {

        char buffer[50] = {0};

        int len, n;


        // Receive message from client

        n = sizeof(servaddr);

        len = recvfrom(sockfd, buffer, 50, MSG_WAITALL, (struct sockaddr *)&servaddr, &n);

        if (len == -1)

        {

            perror("failed to receive");

            continue;

        }

        buffer[len] = '\0';

        printf("Received input: %s\n", buffer);


        // Parse the input

        int a, b, result;

        char op;

        sscanf(buffer, "%d %c %d", &a, &op, &b);


        switch (op)

        {

        case '+':

            result = a + b;

            break;

        case '-':

            result = a - b;

            break;

        case '*':

            result = a * b;

            break;

        case '/':

            result = a / b;

            break;

        default:

            printf("Invalid operator\n");

            result = 0;

        }


        char result_buffer[50];

        sprintf(result_buffer, "%d", result);


        // Send result to client

        len = sendto(sockfd, result_buffer, strlen(result_buffer), 0, (const struct sockaddr *)&servaddr, n);

        if (len == -1)

        {

            perror("failed to send");

            continue;

        }

    }


    close(sockfd);


    return 0;

}