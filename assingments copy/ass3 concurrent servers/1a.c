#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
main()
{
	int sockid;
	int bindid;
	struct sockaddr_in myaddr;
	struct sockaddr_in client;
	int newsockid;
	int clientlen;
        int n;
        char msg[1000];
	int recvid, sendid;
	int port_id = 6000;
	


	sockid = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char*)&myaddr, sizeof(struct sockaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(port_id);
	bindid = bind(sockid, (struct sockaddr*)&myaddr, sizeof(struct sockaddr_in));

	if(bindid < 0)
		printf("error \n");

	listen(sockid, 5);

        clientlen = sizeof(struct sockaddr_in);
        newsockid = accept(sockid, (struct sockaddr*)&client, &clientlen);
		if(newsockid < 0)
			printf("error 2\n");
	        		 
	
        while(1) {
                
					recvid = recvfrom(newsockid, msg, sizeof(msg), 0, (struct sockaddr*)&client, &clientlen);
					sendto(newsockid,"recie",5,0,(struct sockaddr*)&client, sizeof(struct sockaddr_in));
					if(recvid < 0)
						printf("error 2\n");
					    printf("%s \n", msg);
						bzero(msg,1000);
						n=0;
				 
                printf("\n write to the client:");
                while((msg[n++]=getchar())!='\n');
                     sendto(newsockid,msg,sizeof(msg),0,(struct sockaddr*)&client, sizeof(struct sockaddr_in));
                if(strncmp("bye",msg,3)==0)
                  {
                     printf("Exit session...\n");
                     break;
                  }
                }
	return 0;
}



