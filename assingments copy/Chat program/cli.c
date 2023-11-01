#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

main()
{
	int sockid;
	int connectid;
	struct sockaddr_in servaddr;
	struct sockaddr_in client;
	int newsockid;
	int clientlen;
        int n;

	char msg[1000];
	int sendid, recvid;
	int port_id = 6000;
        


	sockid = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char*)&servaddr, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("10.6.7.252");
	servaddr.sin_port = htons(port_id);
	connectid = connect(sockid, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in));

	if(connectid < 0)
		printf("error \n");


        while(1){
			int pid = fork();

			if(pid == 0){
				printf("\n Server: ");
			n=0;
			// bzero(msg,1000);
			while((msg[n++]=getchar())!='\n');
			sendto(sockid,msg,sizeof(msg),0,(struct sockaddr*)&servaddr, sizeof(struct sockaddr_in));
			bzero(msg,1000);
		
			}

        else{
			// bzero(msg,1000);
			recvid = recvfrom(sockid, msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, &clientlen);
				if(recvid < 0)
						printf("error 2\n");
					printf("%s \n", msg);
							bzero(msg,1000);
							n=0;

        if(strncmp("bye",msg,3)==0)
         {
           printf("Exit session...\n");
           break;
         }
		}
        
        } // end of while loop
	return 0;
}
