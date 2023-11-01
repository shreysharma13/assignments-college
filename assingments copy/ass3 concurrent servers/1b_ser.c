#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


int main() {
    int sockid ,  newsockid;
    struct sockaddr_in myaddr;
	struct sockaddr_in cliaddr;
    int clientlen;
    int n;
    char msg[1000];
	// int recvid, sendid;
	int port_id = 6000;

    sockid = socket(AF_INET,SOCK_STREAM,0);

   
    bzero((char*)&myaddr, sizeof(struct sockaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port  = htons(port_id);
    //checks if socket is allocated
	if(bind(sockid, (struct sockaddr*)&myaddr, sizeof(struct sockaddr_in)) < 0)
        printf("Bind error");
        // listens to anyone wanting to connect
    listen(sockid , 5);
    clientlen = sizeof(cliaddr);
    // accepts connection

    newsockid = accept(sockid, (struct sockaddr *) &cliaddr , &clientlen);
    if(newsockid < 0)
    printf("error on accept");


        //reads from client
        while (1)
        {
        bzero(msg,1000);
        n = read(newsockid , msg , 1000);
        sleep(1);
        if(n < 0)
            printf("error on reading");
        printf("Client : %s \n", msg);

        //bye statement
        if(strncmp("BYE" , msg , 3) == 0)
        break;
        //sends to client
        bzero(msg,1000);
        strcpy(msg , "ACK");
        sleep(1);
        printf("%s \n", msg);
        n = write(newsockid ,msg ,strlen(msg));
        //int i = strncmp("BYE" , msg , 3);
        // if( i == 0)
        // break;
        }
    close(newsockid);
    close(sockid);
    return 0;
    
}