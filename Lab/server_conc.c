/*
      GROUP NO.- 4
      NAME:SUSANTA BISWAS       ROLL NO.: 14/CS/25
      NAME:ADITYA SINGH         ROLL NO.: 14/CS/05

      Assignment 2
*/
//Concurrent server code

#include<stdio.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

#define PORT 50025

//evaluates the arithmatic expression sent by the Client
float evaluateExpression(char *exp,int size)
{

   int num=0;
   int i,invalid=0,in=-1;
   float s;
   char op;
   for(i=0;i<size;)
   {
   	if(exp[i]==' ')
      {
      	i++;
      	continue;
      }
      else if(exp[i]=='+' ||exp[i]=='-'||exp[i]=='*'||exp[i]=='/')
      {
          op=exp[i];
         i++;
      }
      else if(exp[i]>=48 && exp[i]<=57)
      {
         while(exp[i]>=48 && exp[i]<=57 && i<size)
         {
         	num=num*10+(exp[i]-48);
         		i++;
         }

         if(in==-1)
         {
         	 in++;
         	 s=num;
		       num=0;
         }
         else
         {

            if(op=='+')
            {
               s=s+num;
            }
            else if(op=='*')
            {
               s=s*num;
            }
            else if(op=='/')
            {
               s=s/num;
            }
            else if(op=='-')
            {
               s=s-num;
            }
            num=0;
         }

      }
   }
  return s;
}

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
   serv_addr.sin_port=htons(PORT);

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
            recv(newsockdesc,buf,100,0);       //for recieving the arithematic expression
         	printf("Expression recieved from Client:%s\n",buf );
		      int n=strlen(buf);
            float ans=evaluateExpression(buf,n);

            sprintf(buf, "%f", ans);

            send(newsockdesc,buf,strlen(buf)+1,0);
            printf("Result sent to the Client successfully\n");
	         for(i=0;i<100;i++)
	            buf[i]='\0';
         }

         close(newsockdesc);
         exit(0);
      }
      close(newsockdesc);
   }
   close(sockdesc);
   return 0;
}
