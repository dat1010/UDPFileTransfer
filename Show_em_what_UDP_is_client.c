
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct numbered_packet {
	int num;
	char data[1024];
};

int main(int argc, char **argv){

//Connection
 	printf("Enter the server IP address: ");
    char server_ip[5000]; 
    fgets(server_ip, 5000, stdin);	
    
    printf("Enter the server port number: ");
    char port_string[5000]; 
    fgets(port_string, 5000, stdin);
	int server_port = atoi(port_string);
											
	//Creating a UDP socket.
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	if(sockfd < 0){
    	printf("There was a problem creating the socket\n");
    	return 1;
  	}
  	
	struct timeval to;
	to.tv_sec = 5;
	to.tv_usec = 0;
	
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(server_port);
	serveraddr.sin_addr.s_addr = inet_addr(server_ip);
	
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof(to));
	
	printf ("Enter a filename: ");
	char line[5000];
	fgets(line, 5000, stdin);
	FILE * fp = fopen ("temp.jpg", "ab");
	char line2[5000];
  	fgets(line2,5000,stdin);
  	 printf (" line: %s", line);
  	 if (strcmp(line, "/exit\n") == 0){
  	 	printf ("Exit message received. Shutting down the client.\n");
  	 	return 1;
  	 }
  	if ( strcmp(line,"") != 0){
  		sendto(sockfd,line,strlen(line),0,
			(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	}
	int bytesReceived = 0;
	char recvBuff[1024];
    memset(recvBuff, '0', sizeof(recvBuff));
    printf ("Buffer: %d \n", sizeof(recvBuff));
	int total_bytes = 0;
  	int len = sizeof(serveraddr);
  	struct numbered_packet * pack;
   	while(bytesReceived = recvfrom(sockfd,pack,1024,0,(struct sockaddr*)&serveraddr,&len) >= 0){
   		printf("Bytes received %d\n",bytesReceived);    
		total_bytes += bytesReceived;
        fwrite(pack->data, 1,bytesReceived,fp);
   	}
   	close (sockfd);
    return 0;
}

