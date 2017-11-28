/*
Assignment 5
/*
      GROUP NO.- A4
      NAME:SUSANTA BISWAS       ROLL NO.: 14/CS/25
      NAME:ADITYA SINGH         ROLL NO.: 14/CS/05

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

int fileHandle;
struct stat obj;
int size,Y;
char command[2][80];

int SD(int code){
	/*
	Create a seperate process
	*/
	if(!fork()){
		int S2;
		struct sockaddr_in serv_addr;
		int exitVal;
		int i;
		// Open socket S2
		if((S2 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("Cannot create socket S2\n");
			exit(0);
		}
		// Bind S1 to port Y
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serv_addr.sin_port = htons(Y);
		sleep(5);
		if((connect( S2, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
			printf("Unable to connect to Client Data\n");
			exit(0);
		}
		printf(" Server Data connnected to Client Data\n");
		if(code == 1) { // command is get
			send( S2, command[1], strlen(command[1]) + 1, 0);	//send filename first
			send( S2, &size, sizeof(int), 0);               	//send size of file
			sendfile( S2, fileHandle, NULL, size);	       		//send file handler
		}else if( code == 2) { // command is put
			int c = 0, len;
			char *f;
			char buff[80];
			for(i=0;i<80;i++) {
				buff[i] = '\0';
			}
			recv( S2, buff, 80, 0);			// receive filename
			int size, filehandle;			// receive size of file
			recv( S2, &size, sizeof(int), 0);
			i = 1;
			filehandle = open(buff, O_CREAT | O_EXCL | O_WRONLY, 0666);
			f = malloc(size);
			recv( S2, f, size, 0);			// get the file
			c = write( filehandle, f, size);
			close(filehandle);
			send( S2, &c, sizeof(int), 0);		// send write status 
			if(c == -1){
				exitVal = 20;
			}else {
				exitVal = 21;
			}
		}
		printf("----------------------Closing Server Data------------------------\n");
		close( S2);
		exit(exitVal);
	}
	int status = 0;
	wait(&status);
	return WEXITSTATUS(status);
}

int main()
{
	int S1, clientfd;
	struct sockaddr_in serv_addr, cli_addr;
	int clientLen;
	
	int bufLen;
	char buff[80];
	int i,first = 0,j,k;
	int serverResponse = 0;
	
	// Open socket S1
	if((S1 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Cannot create socket S1\n");
		exit(0);
	}
	// Bind S1 to port X, ie, 9999
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(9999);
	if(bind( S1, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("Unable to bind to port X\n");
		exit(0);
	}
	// Queue max 5 clients
	listen( S1, 5);
	
	while(1) {
		printf("Server Control running ..... Waiting for Client Control \n");
		first = 0;
		clientLen = sizeof(cli_addr);
		clientfd = accept( S1, (struct sockaddr *)&cli_addr, &clientLen);
		if(clientfd < 0){
			printf("Unable to accept Client Control\n");
			exit(0);
		}
		
		printf(" Client control connected\n");
		bufLen = 1;
		while(bufLen) {
			for(i=0;i<80;i++) {
				buff[i] = '\0';
				command[0][i] = '\0';
				command[1][i] = '\0';
			}
			bufLen = recv( clientfd, buff, 80, 0);
			
			/*
			Parse the command
			*/
			j=0;
			k=0;	
			int flag = 0;			
			for( i=0;i<bufLen;i++){
				while(buff[i] == ' ' || buff[i] == '\n' || buff[i] == '\0'){
					j++;
					k=0;
					i++;
				}
				if(i>=bufLen){
					break;
				}
				if( j >= 2) {
					serverResponse = 501;
					send(clientfd, &serverResponse, sizeof(int), 0);
					flag = -999;
				}else {
					command[j][k++] = buff[i];
				}
			} 
			
			// ignore commands with invalid argument list
			if( flag == -999){
				continue;
			}
			
			/*
			Check whether this is the first command 
			*/
			if(!first) {
				if(strcmp(command[0],"port")){
					// The first command is not port
					serverResponse = 503;
					send(clientfd, &serverResponse, sizeof(int), 0);
				}else {
					Y = atoi(command[1]);
					printf("%d\n",Y);
					if(Y >= 1024 && Y <= 65535) {
						first = 1;
						serverResponse= 200;				// success
						send( clientfd, &serverResponse, sizeof(int), 0);
					} else {
						serverResponse = 550;
						send(clientfd, &serverResponse, sizeof(int), 0);
					}
				}
			} else {
				if(!strcmp( command[0], "cd")) {
					if( !chdir( command[1])){
						serverResponse = 200;				// success
						send( clientfd, &serverResponse, sizeof(int), 0);
					}else {
						serverResponse = 501;
						send( clientfd, &serverResponse, sizeof(int), 0);
					}
				}else if(!strcmp( command[0], "get")) {
					stat( command[1], &obj);
					fileHandle = open( command[1], O_RDONLY);
					size = obj.st_size;
					if(fileHandle == -1) { // no such file
						serverResponse = 550;
						send( clientfd, &serverResponse, sizeof(int), 0);
					}else {	// file exists 
						serverResponse = 999;				// send response that file exists
						send( clientfd, &serverResponse, sizeof(int), 0);
						SD(1);
						serverResponse = 250;				// success
						send( clientfd, &serverResponse, sizeof(int), 0);
					}
				}else if(!strcmp( command[0], "put")) {
					int filePresent = 0;
					recv( clientfd, &filePresent, sizeof(int), 0);
					if( filePresent == 100) {
						if(SD(2) == 21){
							serverResponse = 250;				// success
							send( clientfd, &serverResponse, sizeof(int), 0);
						}else {
							serverResponse = 550;
							send( clientfd, &serverResponse, sizeof(int), 0);
						}
					}else if(filePresent == 99) {
					}
				}else if(!strcmp( command[0], "quit")) {
					serverResponse = 421;
					send( clientfd, &serverResponse, sizeof(int), 0);
					break;
				}else {
					serverResponse = 502;
					send( clientfd, &serverResponse, sizeof(int), 0);
				}
			}		
				
		}// end of while(bufLen)
		printf("---------------------------Closing Client Control --------------------------------------------\n");
		close(clientfd);
	}
	printf("---------------------------Closing Server Control --------------------------------------------\n");
	close(S1);
	return 0;
}// end of main