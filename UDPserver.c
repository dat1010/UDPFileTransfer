#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct sigaction exit_action;
bool run = true;


struct numbered_packet {
	int num;
	char data[1024];
};

void sig_handler(int sig_num){
	printf ("Shutting the server down. Received Ctrl-C.\n");
	run = false;
}

int main(int argc, char **argv){

    printf("Enter the server port number: ");
    char port_string[5000];
    fgets(port_string, 5000, stdin);
		int server_port = atoi(port_string);
		struct sockaddr_in serveraddr,cliaddr;

	//Creating a UDP socket.
	int sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0){
    	printf("There was a problem creating the socket\n");
    	return 1;
	}



	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(server_port);


	if (bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
	{
  		printf ("Error in binding.");
  		return 1;
  }

	while (1)
	{
		printf ("Receiving from socket.... \n");
    int len = sizeof(cliaddr);
    char line[5000];

    if (recvfrom(sockfd,line,5000,0,(struct sockaddr*)&cliaddr,&len) < 0)
		{
    	 printf("Error in receiving.\n");
    }

		printf("Got from the client: %s\n", line);

		char *fileName = strtok(line, "\n");

		FILE *fp = fopen(fileName, "r");

		char sdbuf[1024];
		if(fp == NULL)
    {
    		printf("ERROR: File %s not found.\n", fileName);
        exit(1);
    }
  	//bzero(sdbuf, 1024);


    int f_block_sz;
    int total_sent = 0;
    int pack_num = 0;
    struct numbered_packet *to_send;

		//We need to alocate memory, getting segfault
		printf("this is Test %i\n",f_block_sz);
    while((f_block_sz = fread(to_send->data, sizeof(fp), 1024, fp)) > 0)
    {
			printf("f_block_sz = %c\n", f_block_sz);

        to_send->num = pack_num;
        if (sendto(sockfd,to_send,f_block_sz,0,(struct sockaddr*)&cliaddr,sizeof(cliaddr)) < 0)
        {
            printf("ERROR: Failed to send file %s.\n", fileName);
        }
        else
				{
            printf ("Sent %d bytes \n", f_block_sz);
						total_sent += f_block_sz;
			   	  (sdbuf, 1024);
				}

				pack_num++;
				printf("Number of packets sent %i\n",pack_num);

        }
        printf ("Sent %d total.\n", total_sent);
    }
    close(sockfd);
    return 0;
}
