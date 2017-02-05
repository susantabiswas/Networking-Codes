#include<stdio.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

int main()
{
   //socket descriptors
   int sockdesc,newsockdesc;
   int client_len;
   struct sockaddr_in client_addr,serv_addr;
   char buf[100];   //for storing the recieved messages

   int i;
   //create a socket
   sockdesc=socket(AF_INET,SOCK_STREAM,0);

   //if sockdesc returns <0 then it is an error
   if(sockdesc<0)
   {
      printf("Socket creation failed \n");
      exit(0);
   }

   serv_addr.sin_family=AF_INET;
   serv_addr.sin_addr.s_addr=INADDR_ANY;
   serv_addr.sin_port=htons(5642);

   //now if the socket creation is successful ,then next is binding the socket
   if(bind(sockdesc,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
   {
      printf("Unable to bind the address\n");
      exit(0);
   }

   listen(sockdesc,5);

   while(1)
   {

      client_len=sizeof(client_addr);
      newsockdesc=accept(sockdesc,(struct sockaddr*) &client_addr,&client_len);

      if(newsockdesc<0)
      {
         printf("Accept error\n");
         exit(0);
      }

      if(fork()==0)
      {
         close(sockdesc);

         while(1)
         {
         	strcpy(buf,"Message from server");
	         send(newsockdesc,buf,strlen(buf)+1,0);
	         for(i=0;i<100;i++)
	            buf[i]='\0';
	         recv(newsockdesc,buf,100,0);
	         printf("%s\n",buf);
         }

         close(newsockdesc);
         exit(0);
      }
      close(newsockdesc);
   }
   close(sockdesc);
   return 0;
}
