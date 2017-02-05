//Client code 

#include<stdio.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

#define PORT 50025

int main()
{
   //socket descriptor
   int sockdesc;

   struct sockaddr_in serv_addr;
   int i;
   char buf[100];

   sockdesc=socket(AF_INET,SOCK_STREAM,0);
   if(sockdesc<0)
   {
      printf("Socket creation failed \n");
      exit(0);
   }

   serv_addr.sin_family=AF_INET;
   serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
   serv_addr.sin_port=htons(PORT);

   //now if the socket creation is successful ,then next is binding the socket
   if(connect(sockdesc,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
   {
      printf("Unable to bind the address\n");
      exit(0);
   }
   else
      printf("Connection to server Successful.......\n");
   while(1)
   {
 	for(i=0; i < 100; i++) 
		buf[i] = '\0';
      
      printf("\nEnter an Arithematic expression\n");
   	gets(buf);

      if(buf[0]=='-' && buf[1]=='1')
      {
         printf("Client Disconnected\n");
          exit(0);
      }
   	send(sockdesc, buf, strlen(buf) + 1, 0);      //send the arithematic expression to the server 
	   recv(sockdesc, buf, 100, 0);                  //for recieving the result of the arithematic expression
		
      printf("Result from Server:%s\n", buf);
   }
  
   send(sockdesc, buf, strlen(buf) + 1, 0);
   close(sockdesc);

   return 0;
}
