#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

#define SIZE 1024
#define PORT 9898

void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recvOnClient.txt";
  char buffer[SIZE];
  n=0;
  fp = fopen(filename, "w");
  while (n<10) {
    recv(sockfd, buffer, SIZE, 0);
    fprintf(fp, "%s", buffer);
    bzero(buffer, sizeof(buffer));
    n++;
  }
  fclose(fp);
  return;
}

int main(){
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET,SOCK_STREAM,0);
	if(clientSocket<0){
		printf("Error in connection @socket creation.\n");
		exit(1);
	}else{
		printf("Socket is created.\n");	
	}

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket,(struct sockaddr*) &serverAddr,sizeof(serverAddr));
	if(ret<0){
		printf("Error in connection @connect.\n");
		exit(1);
	}else{
		printf("connected to server.\n");
	}

	printf("Enter Data ");
	scanf("%s", &buffer[0]);
	send(clientSocket,buffer,strlen(buffer),0);

	write_file(clientSocket);

	ifstream is("recvOnClient.txt");
  
  string histr;
  getline(is,histr);

  // sscanf(histr.c_str(), "%d %d %d %d", &w, &x, &y, &z);

  printf("HCP--> %s.\n", histr.c_str());
  // send(clientSocket,histr,strlen(histr),0);
  // bzero(buffer,sizeof(buffer));
  recv(clientSocket,buffer,1024,0);

  if(strcmp(buffer,"Bhai_sabse_jyda_cpu_use_bhejde")==0){
  	string message = "Bhej_Rha.\n";
  	send(clientSocket,message.c_str(),message.size(),0);
  	send(clientSocket,histr.c_str(),histr.size(),0);	
  }
  
  printf("clientSocket Closed.\n");
  close(clientSocket);  

	// while(1){
	// 	printf("Enter N ");
	// 	scanf("%s", &buffer[0]);
	// 	send(clientSocket,buffer,strlen(buffer),0);

	// 	if(strcmp(buffer,":exit")==0){
	// 		close(clientSocket);
	// 		printf("disconnect from server.\n");
	// 		exit(1);
	// 	}

	// 	if(recv(clientSocket,buffer,1024,0)<0){
	// 		printf("error in receiving data./n");
	// 	}else{
	// 		printf("Server:\t%s\n",buffer);
	// 	}
	// }

	return 0;
 }