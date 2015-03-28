#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct sigaction exit_action;
bool run = true;

struct sockaddr_in temp_client;

struct numbered_packet {
	int num;
	char data[1024];
};

void sig_handler(int sig_num){
	printf ("Shutting the server down. Received Ctrl-C.\n");
	run = false;
}

int main(int argc, char **argv){

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
	
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(server_port);
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	
	//setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof(to));
	
	if (bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0) {
  		printf ("Error in binding.");
  		return 1;
  	}
	
	while (1){
		printf ("Receiving from socket.... \n");
    	int len = sizeof(temp_client);
    	char line[5000];
    	if (recvfrom(sockfd,line,5000,0,
    	  	(struct sockaddr*)&temp_client,&len) < 0){
    	 	printf ("Error in receiving.\n");
    	}
		printf ( "Got from the client: %s\n", line);
		FILE * fp = fopen (line, "r");
		char sdbuf[1024];
		if(fp == NULL)
          {
                printf("ERROR: File %s not found.\n", line);
                exit(1);
          }
        bzero(sdbuf, 1024);
        
        int f_block_sz;
        int total_blah = 0;
        int pack_num = 0;
        struct numbered_packet * to_send;
        while((f_block_sz = fread(to_send->data, sizeof(char), 1024, fp))>0)
        {
        	to_send->num = pack_num;
        	if (sendto(sockfd,to_send,f_block_sz,0,(struct sockaddr*)&temp_client,sizeof(temp_client)) < 0)
         	{
                   printf("ERROR: Failed to send file %s.\n", line);
         	}
         	else {
            	   printf ("Sent %d bytes \n", f_block_sz);
               	   total_blah += f_block_sz;
			   	   bzero(sdbuf, 1024);
			}
			pack_num++;
        }
        printf ("Sent %d total", total_blah);
    }
    close(sockfd);
    return 0;
}

