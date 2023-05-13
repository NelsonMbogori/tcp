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

    {        perror("failed to create socket");


        exit(EXIT_FAILURE);


    }    // Set server address and port

    servaddr.sin_family = AF_INET;


    servaddr.sin_port = htons(PORT);


    // Set server IP address


  if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0)


  {


 perror("failed to set server IP address");


 exit(EXIT_FAILURE);


}// Prompt user to enter two numbers and an operation


char input[50] = {0};


printf("Enter two numbers and an operation (+, -, *, /) separated by spaces:\n");


fgets(input, 50, stdin);


// Send input to server


int len = sendto(sockfd, (const char *)input, strlen(input), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));


if (len == -1)

{

    perror("failed to send input to server");

}

// Receive result from server

char result[50] = {0};

len = recvfrom(sockfd, (char *)result, 50, 0, NULL, NULL);

if (len == -1)

{

    perror("failed to receive result from server");

}

printf("Result: %s\n", result);




// Close socket

close(sockfd);

return 0;

}
