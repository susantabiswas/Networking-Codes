/*
      GROUP NO.- 4
      NAME:SUSANTA BISWAS       ROLL NO.: 14/CS/25
      NAME:ADITYA SINGH         ROLL NO.: 14/CS/05

      Assignment 1
*/
//Iterative Server Code

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

#define PORT 50025

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
         printf("%s\n",client_message);
         char news[101];
         client_message[strlen(client_message)]='+' ;
        int i;
        int counter=0;
  for(i=0;i<strlen(client_message); i++)
  {
    if(client_message[i]==' ')
    continue;
    else
    {
    news[counter] = client_message[i];
    counter++;
    }
  }
  news[counter]='\0';
   int stack[1000]={0};
   int top=0;
   char value[101];
   counter=0;
   int insert;

   //expression evaluation
   for(int i=0;i<strlen(news); i++)
   {
      if(news[i]=='+')
      {
         value[counter]='\0';
         counter=0;
         sscanf(value, "%d", &insert);
         stack[top]=insert;
         top++;
         stack[top]=-1;
         top++;
      }

      else if(news[i]=='-')
      {
         value[counter]='\0';
         counter=0;
         sscanf(value, "%d", &insert);
         stack[top]=insert;
         top++;
         stack[top]=-2;
         top++;
      }

      else if(news[i]=='*')
      {
         value[counter]='\0';
         counter=0;
         sscanf(value, "%d", &insert);
         stack[top]=insert;
         top++;
         stack[top]=-3;
         top++;
      }

      else if(news[i]=='/')
      {
         value[counter]='\0';
         counter=0;
         sscanf(value, "%d", &insert);
         stack[top]=insert;
         top++;
         stack[top]=-4;
         top++;
      }
      else
      {
        value[counter]=news[i];
        counter++;
      }
   }

    i=0;
    float ans=0;
    ans=stack[i];
    while(i<=top-2)
    {
      i++;
      int indicator = stack[i];
      if(indicator==-1)
      {
        i++;
        ans +=stack[i];
      }
      else if(indicator==-2)
      {
        i++;
        ans -=stack[i];
      }
      else if(indicator==-3)
      {
        i++;
        ans *=stack[i];
      }
      else if(indicator==-4)
      {
        i++;
        ans /=stack[i];
      }
    }

     memset(client_message, 0, 255);
     char send_message[100];
     top=0;
     sprintf(send_message, "%f", ans);
     printf("Result sent to the Client successfully\n");
     write(client_sock , send_message , strlen(send_message));
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recieve failed");
    }

    return 0;
}
