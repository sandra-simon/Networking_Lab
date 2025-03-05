// Server-client application in which client sends a  number and server checks if it is prime, else returns the factors of the composite number.


#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
void isprime(int num,char *response)
{
      if(num<2)
      {
          //sprintf(response,"%d is neither prime nor composite\n",num);
          return;
      }
      int prime=1;
      char factors[1024]="";
      int count=0;
      for(int i=2;i<=num/2;i++)
      {
          if(num%i==0)
          {
               prime=0;
               char temp[40];
               sprintf(temp,"%d ",i);
               strcat(factors,temp);
               count++;
          }
      }

       if(prime)
       {
             sprintf(response,"%d is PRIME.\n",num);
       }
      else
      {
           sprintf(response,"%d is COMPOSITE...factors are : %s\n",num,factors);
      }
}

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
      perror("couldnot create socket!\n");
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
       printf("UDP Server is running\n");

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
          printf("Client : %s\n",buffer);
         if(strcmp("exit",buffer)== 0)
         {
            printf("client exited...\n");
            break;
         }
           int num=atoi(buffer);
           char response[1024];
           isprime(num,response);
           printf("Server : %s",response);
           sendto(server_socket,response,strlen(response),0,(struct sockaddr *)&clientaddr,addrSize);
           if(strcmp(buffer,"exit")== 0)
           {
              printf("Server exiting...\n");
              break;
           }
       }
   close(server_socket);
   return 0;
}
