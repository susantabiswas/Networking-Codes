#include<stdio.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

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
   serv_addr.sin_port=htons(5642);

   //now if the socket creation is successful ,then next is binding the socket
   if(connect(sockdesc,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
   {
      printf("Unable to bind the address\n");
      exit(0);
   }

   while(1)
   {
  	 	for(i=0; i < 100; i++) 
   			buf[i] = '\0';
   		fgets(buf,100,stdin);
   		send(sockdesc, buf, strlen(buf) + 1, 0);
		recv(sockdesc, buf, 100, 0);
		printf("%s\n", buf);
   }
  
  //recv(sockdesc, buf, 100, 0);
  
   //strcpy(buf,"Message from client");
   send(sockdesc, buf, strlen(buf) + 1, 0);
   close(sockdesc);

   return 0;
}
