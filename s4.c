#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen, pid;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0)
        {
            error("ERROR on accept");
        }

        pid = fork(); //fork new process

        if (pid < 0)
        {
            error("ERROR on fork");
        }

        if (pid == 0) //child process
        {
            close(sockfd); //close original socket in child process

            int num1, num2, choice, ans;

            do
            {
                //request first number
                n = write(newsockfd, "Enter first number: ", strlen("Enter first number: "));
                if (n < 0)
                {
                    error("error writting to socket");
                }
                read(newsockfd, &num1, sizeof(int));
                printf("client's first number is: %d\n", num1);

                //request second number
                n = write(newsockfd, "Enter second number: ", strlen("Enter second number: "));
                if (n < 0)
                {
                    error(" error writting to socket");
                }
                read(newsockfd, &num2, sizeof(int));
                printf("client's second number is: %d\n", num2);

                ///////request and receive choice
//request choice
n = write(newsockfd, "Enter your choice: \n 1. Addition\n 2. Subtraction\n 3. Multiplication\n 4. Division\n 5. Exit\n: ", strlen("Enter your choice: \n 1. Addition\n 2. Subtraction\n 3. Multiplication\n 4. Division\n 5. Exit\n:"));
if (n < 0)
{
    error("Error writing to socket");
}

//read choice
read(newsockfd, &choice, sizeof(int));
printf("Client's choice is: %d\n", choice);

switch (choice)
{
    case 1:
        ans = num1 + num2;
        break;

    case 2:
        ans = num1 - num2;
        break;

    case 3:
        ans = num1 * num2;
        break;

    case 4:
        ans = num1 / num2;
        break;

    case 5:
        goto Q;
        break;
}

// Send the answer back to client
write(newsockfd, &ans, sizeof(int));
} while (choice != 5);

close(newsockfd);
exit(0); //exit child process
} else {  //parent process
close(newsockfd); //closing socket in parent process
}
}

Q:
close(sockfd);
return 0;
}

