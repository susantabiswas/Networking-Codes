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

char command[2][80];
int Y;
int filehandle;

void CD(int code){
	/*
	Create a new process
	*/
	if(!fork()){
		int C2, clientfd;
		int i;
		struct sockaddr_in serv_addr, cli_addr;
		int clientLen;
		// Open socket C2
		if((C2 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("Cannot create socket S1\n");
			exit(0);
		}
		// Bind C2 to port Y
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(Y);
		if(bind( C2, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
			printf("Unable to bind to port Y\n");
			exit(0);
		}
		// Queue max 1 clients
		listen( C2, 5);
		printf("Client Data running ..... Waiting for Server Data on port %d \n", Y);
		clientLen = sizeof(cli_addr);
		clientfd = accept( C2, (struct sockaddr *)&cli_addr, &clientLen);
		if(clientfd < 0){
			printf("Unable to accept Client Control\n");
			exit(0);
		}
		printf(" Server data connected\n");
		if(code == 1){ // get command
			char buff[80];
			int size;
			char *f;
			int c = 0;
			int filehandle; 
			for(i=0;i<80;i++) {
				buff[i] = '\0';
			}
			recv( clientfd, buff, 80, 0);		// get file name
			recv( clientfd, &size, sizeof(int), 0);	// get file size
			f = malloc(size);
			recv( clientfd, f, size, 0);
			filehandle = open(buff, O_CREAT | O_EXCL | O_WRONLY, 0666);
			c = write( filehandle, f, size);
			close( filehandle);
			char *s = "cat ";
			strcat( s, buff);
			system( s);
		}else if(code == 2) { // put command
			struct stat obj;
			int size;
			stat( command[1], &obj);
			size = obj.st_size;
			send( clientfd, command[1], strlen(command[1]) + 1, 0);
			send( clientfd, &size, sizeof(size), 0);
			sendfile( clientfd, filehandle, NULL, size);
		}
		printf("--------------------CLosing Client Data-------------------------\n");
		close(clientfd);
		close(C2);
		exit(0);
	}
}

int main()
{
	int C1;
	struct sockaddr_in serv_addr;
	
	char buff[80];
	int i;
	int serverResponse = 0;
	
	// Open socket C1
	if(( C1 = socket( AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Unable to create socket C1\n");
		exit(0);
	}
	// Connect C1 to port X, ie, 9999
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(9999);
	if((connect( C1, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
		printf("Unable to connect to Server Control\n");
		exit(0);
	}
	
	printf("Client Control connected to Server Control \n");
	while(1) {
		for(i=0;i<80;i++) {
			buff[i] = '\0';
			command[0][i] = '\0';
			command[1][i] = '\0';
		}
		printf(">");
		// take command and act 
		gets(buff);
		int j,k;
		j=0;
		k=0;				
		for( i=0;i<strlen(buff);i++){
			while(buff[i] == ' ' || buff[i] == '\n' || buff[i] == '\0'){
				j=1;
				k=0;
			i++;
			}
			if(i>=strlen(buff)){
				break;
			}
			command[j][k++] = buff[i];
		} 
		printf("--------Sent %d bytes to server -----------\n", (send( C1, buff, strlen(buff) + 1, 0)));
		if(!strcmp( command[0], "get")) {
			recv( C1, &serverResponse, sizeof(int), 0);
			if( serverResponse != 550) {
				CD(1);
			}else {
				printf("Requested file not found\n");
				continue;
			}
		}else if(!strcmp( command[0], "put")){
			filehandle = open( command[1], O_RDONLY);
			if( filehandle == -1) {
				int filePresent = 99;
				send( C1, &filePresent, sizeof(int), 0);
				printf("File not present in local directory\n");
				continue;
			}else {
				int filePresent = 100;
				send( C1, &filePresent, sizeof(int), 0);
				CD(2);
			}
		}else if(!strcmp( command[0], "port")) {
			Y = atoi(command[1]);
		}
		int status = 0;
		wait(&status);
		for(i=0;i<80;i++) {
			buff[i] = '\0';
		}
		recv( C1, &serverResponse, sizeof(int), 0);
		printf("%d\n", serverResponse);
		if( serverResponse == 200 || serverResponse == 250) {
			printf("Successful\n");
		}else if( serverResponse == 421) {
			break;
		}else if( serverResponse == 503) {
			printf("First command should be port Y\n");
		}else if( serverResponse == 550) {
			printf("Incorrect port OR Requested file not found\n");
		}else if( serverResponse == 501) {
			printf("Change directory not successful\n");
		}else if( serverResponse == 502) {
			printf("Command not supported\n");
		}
	}
	printf("--------------------------CLosing Client Control-------------------------------\n");
	close(C1);
	return 0;
}