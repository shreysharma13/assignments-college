#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){

    int sockid  , n;
    struct sockaddr_in ser_addr;
    int connectid;
    char msg[1000];
    int portno = 6000;

    sockid = socket( AF_INET ,SOCK_STREAM, 0);
    

	bzero((char*)&ser_addr, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ser_addr.sin_port = htons(portno);
	connectid = connect(sockid, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in));

	if(connectid < 0)
		printf("error \n");
        
        if (sockid < 0)
        printf("error opening socket");
        bzero(msg,1000);
        strcpy(msg,"HELLO");
         printf("%s",msg);
        sleep(1);
        n = write(sockid, msg , strlen(msg));
        if (n < 0)
        printf("error on write");
        bzero(msg,1000);
        sleep(1);
        n = read(sockid , msg , 1000);
        if (n < 0)
        printf("error on read ");
        printf("Server : %s \n" , msg);

        // int i = strncmp("BYE" , msg , 3);
        // if( i == 0)
        // break;
    close(sockid);
    return 0;


}