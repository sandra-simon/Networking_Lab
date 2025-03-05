#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
   int client_socket,status;
   char buffer[1024];
   struct sockaddr_in servaddr;
   struct sockaddr_storage store;
   socklen_t  addrSize=sizeof(servaddr);
   client_socket=socket(AF_INET,SOCK_DGRAM,0);

    if (client_socket<0)
   {
       perror("Socket creation failed\n");
       return 1;
   }


   servaddr.sin_family=AF_INET;
   servaddr.sin_port=htons(6165);
   servaddr.sin_addr.s_addr=inet_addr("127.0.0.28");

    printf("connected to udp server\n");
    while(1)
    {
          memset(buffer,0,sizeof(buffer));
          printf("Enter number : ");
          fgets(buffer,sizeof(buffer),stdin);
          buffer[strcspn(buffer,"\n")]='\0';
          sendto(client_socket,buffer,strlen(buffer),0,(struct sockaddr *)&servaddr,addrSize);
          if(strcmp(buffer,"exit")==0)
          {
              printf("client exiting...\n");
             break;
          }
          int recvstatus= recvfrom(client_socket,buffer,sizeof(buffer)-1,0,NULL,NULL);
          if(recvstatus<0)
          {
              printf("recieving failed\n");
              continue;
          }
             buffer[recvstatus]='\0';
             printf("Server : %s\n",buffer);

          if(strcmp("exit",buffer)==0)
          {
              printf("server exited...\n");
              break;
          }
    }
    close(client_socket);
    return 0;
}
