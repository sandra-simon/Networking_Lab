#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
   int server_socket,status;
   char buffer[1024];
   struct sockaddr_in servaddr,clientaddr;
   struct sockaddr_storage store;
   socklen_t  addrSize=sizeof(clientaddr);
   server_socket=socket(AF_INET,SOCK_DGRAM,0);
    if(server_socket<0)
    {
      perror("could not create socket!\n");
      return 1;
    }
   servaddr.sin_family=AF_INET;
   servaddr.sin_port=htons(6165);
   servaddr.sin_addr.s_addr=inet_addr("127.0.0.28");
   if (bind(server_socket,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
   {
        printf("Socket binding failed\n");
        exit(0);
    }
   else
        printf("Socket Successfully Binded\n");
        printf("UDP Server is running..\n");

    while(1)
    {
       memset(buffer,0,sizeof(buffer));

       int recvstatus= recvfrom(server_socket,buffer,sizeof(buffer)-1,0,(struct sockaddr *)&clientaddr,&addrSize);
         if(recvstatus<0)
         {
              printf("Recieve failed\n");
              continue;
         }

          buffer[recvstatus]='\0';
          printf("Client: %s\n",buffer);
         if(strcmp("exit",buffer)== 0)
         {
            printf("client exited...\n");
            break;
         }
          printf("Server: ");
          fgets(buffer,sizeof(buffer),stdin);
          buffer[strcspn(buffer,"\n")]='\0';
          sendto(server_socket,buffer,strlen(buffer),0,(struct sockaddr *)&clientaddr,addrSize);
          if(strcmp(buffer,"exit")== 0)
          {
             printf("exiting server...\n");
             break;
          }
       }
   close(server_socket);
   return 0;
}
