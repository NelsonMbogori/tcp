#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[]) {
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[300];

	if (argc < 3) {
    	fprintf(stderr, "usage %s hostname port\n", argv[0]);
    	exit(1);
	}

	portno = atoi(argv[2]);
// Step 1: Create a socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
    	error("socket opening error");
	}

	server = gethostbyname(argv[1]);
	if (server == NULL) {
    	fprintf(stderr, "no such host");
    	exit(0);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
   // Step 2: Fill in the address of the server

	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
   // Step 3: Connect to the server

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    	error("Failed to connect");
	}

	int num1, num2, ans, choice;

	S:
	bzero(buffer, 300);
	n = read(sockfd, buffer, 300);
	if (n < 0) {
    	error("error reading from socket");
	}
	printf("server - %s\n", buffer);
	scanf("%d", &num1);
	write(sockfd, &num1, sizeof(int));

	bzero(buffer, 300);
	n = read(sockfd, buffer, 300);
	if (n < 0) {
    	error("error reading from socket");
	}
	printf("server - %s\n", buffer);
	scanf("%d", &num2);
	write(sockfd, &num2, sizeof(int));

	bzero(buffer, 300);
	n = read(sockfd, buffer, 300);
	if (n < 0) {
    	error("error reading from socket");
	}
	printf("server - %s\n", buffer);
	scanf("%d", &choice);
	write(sockfd, &choice, sizeof(int));
	if (choice == 5) {
    	goto Q;
	}

	read(sockfd, &ans, sizeof(int));
	printf("the answer is %d\n", ans);

Q:
	printf("exit successful\n");
	close(sockfd);
	return 0;
}

