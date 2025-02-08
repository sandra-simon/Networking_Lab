#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>
void capitalize(char *str)
{
    int i=0,flag=1;
   while(str[i])
   {
      if(isalpha(str[i]) && flag)
      {
           str[i]=toupper(str[i]);
           flag=0;
      }
      else if(isspace(str[i]))
      {
           flag=1;
      }
      i++;
   }
}
int main()
{
   int server_socket,newsocket,status;
   char buffer[1024];
   struct sockaddr_in servaddr,clientaddr;
   struct sockaddr_storage store;
   socklen_t  addrSize;
   server_socket=socket(AF_INET,SOCK_STREAM,0);
    if(server_socket==-1)
    {
      perror("could not create socket!\n");
      return 1;
    }
   servaddr.sin_family=AF_INET;
   servaddr.sin_port=htons(6160);
   servaddr.sin_addr.s_addr=inet_addr("127.0.0.28");
   if (bind(server_socket,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
   {
        printf("Socket binding failed\n");
        exit(0);
    }
   else
       printf("Socket Successfully Binded\n");

    if(listen(server_socket,5)!=0)
    {
       printf("Listening failed\n");
       exit(0);
    }
   else
       printf("Server Listening...\n");

  while(1)
  {
      newsocket=accept(server_socket,(struct sockaddr *)&store,&addrSize);
      if(newsocket<0)
      {
         printf("accepting failed\n");
         continue;
      }
      printf("Connected to client...\n");
    while(1)
    {
       memset(buffer,0,sizeof(buffer));
       int recvstatus= recv(newsocket,buffer,sizeof(buffer)-1,0);
       if(recvstatus>0)
       {
          buffer[recvstatus]='\0';
          printf("Client: %s\n",buffer);
          if(strcmp("exit",buffer)== 0)
          {
             printf("client exited!\n");
             break;
          }
            capitalize(buffer);
           printf("Modified message: %s\n",buffer);
           send(newsocket,buffer,strlen(buffer),0);
      }
      else if(recvstatus==0)
      {
         printf("Client Disconnected\n");
         break;
      }
      else
      {
        printf("Receiving Failed\n");
        break;
      }
   }

    close(newsocket);
    printf("Waiting for new client...\n");
 }
   close(server_socket);
   return 0;
}
