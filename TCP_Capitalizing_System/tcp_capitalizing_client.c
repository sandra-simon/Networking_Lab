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
   socklen_t  addrSize;
   client_socket=socket(AF_INET,SOCK_STREAM,0);
   servaddr.sin_family=AF_INET;
   servaddr.sin_port=htons(6160);
   servaddr.sin_addr.s_addr=inet_addr("127.0.0.28");
   if (connect(client_socket,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
   {
       perror("connection failed\n");
       return 1;
   } 
    printf("connected to server\n");
    while(1)
    {
          memset(buffer,0,sizeof(buffer));
          printf("Enter data to send to server: ");
          fgets(buffer,sizeof(buffer),stdin);
          buffer[strcspn(buffer,"\n")]='\0';
          send(client_socket,buffer,strlen(buffer),0);
          if(strcmp(buffer,"exit")==0)
          {
              printf("client exiting...\n");
             break;
          } 
          int recvstatus= recv(client_socket,buffer,sizeof(buffer)-1,0);
          if(recvstatus>0)
          {
              buffer[recvstatus]='\0';
             printf("Modified message from server: %s\n",buffer);
          }
          else
         {
            printf("Recieve failed\n");
            break;
         }
    }
    close(client_socket);
    return 0;
}
