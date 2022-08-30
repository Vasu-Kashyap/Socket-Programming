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

void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, sizeof(data));
  }
  printf("[+]file sent successfully.\n");
}

int is_pid_folder(const struct dirent *entry) {
    const char *p;

    for (p = entry->d_name; *p; p++) {
        if (!isdigit(*p))
            return 0;
    }

    return 1;
}
typedef struct processes
{
	char Name[100];
    int pid;
    int utime;
    int stime;
    int sum ;
    
}Process; 

bool compare( Process a, Process b){
    if(a.sum > b.sum)
        return 1;
    else 
        return 0;
}
int findTopN(int n) {
    DIR *procdir;
    FILE *fp;
    struct dirent *entry;
    char path[256 + 10 + 10];
    int pid;
    unsigned long utime;
    unsigned long stime;
    Process p_arr[5000];

    ofstream hout("check.txt");
    
    procdir = opendir("/proc");
    if (!procdir) {
        perror("opendir failed");
        return 1;
    }
    int k = 0;
    while ((entry = readdir(procdir))) {
        if (!is_pid_folder(entry))
            continue;

        
        snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);
        fp = fopen(path, "r");

        if (!fp) {
            perror(path);
            continue;
        }
        char name[100];
        fscanf(fp, "%d %s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu",
            &pid,name, &utime, &stime
        );
        strcpy(p_arr[k].Name,name);
        p_arr[k].pid = pid;
        p_arr[k].utime =utime;
        p_arr[k].stime =stime;
        p_arr[k].sum =utime+stime; 
        k++;
        // printf("%5d %-20lu: %lu\n", pid, utime, stime);
        fclose(fp);
    }
    
    sort(p_arr, p_arr+k, compare);

    for(int i=0;i<n;i++){
    	hout<<p_arr[i].Name <<"   ";
        hout<<p_arr[i].pid <<"   " ;
        hout<<p_arr[i].utime << "  ";
        hout<<p_arr[i].stime << "  ";
        hout<<p_arr[i].sum << endl;
    }
    hout.close();
    closedir(procdir);
    return 0;
}
int main(){
	int sockfd,ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;
	char buffer[1024];
	pid_t childpid;

	FILE *fp;
  	char *filename = "check.txt";

	sockfd= socket(AF_INET, SOCK_STREAM,0);
	if(sockfd<0){
		printf("Error in connection @socket creation.\n"); 	
	}else{
		printf("socket created successfully.\n");
	}

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd,(struct sockaddr*) &serverAddr,sizeof(serverAddr));
	if(ret<0){
		printf("Error in connection @bind.\n");
		exit(1);
	}else{
		printf("Bind to port %d.\n",9898);
	}

	if(listen(sockfd,10)==0){
		printf("Server now listening...\n");
	}else{
		printf("Error in connection @listen.\n");
	}

	while(1){
		newSocket = accept(sockfd,(struct sockaddr*) &newAddr,&addr_size);
		if(newSocket <0){
			exit(1);
		}
		printf("connection accepted from %s:%d\n",inet_ntoa(newAddr.sin_addr),ntohs(newAddr.sin_port));
		
		if((childpid =fork())==0){
			close(sockfd);
			
			bzero(buffer,sizeof(buffer));
			recv(newSocket,buffer,1024,0);
			printf("%s",buffer);

			if(strcmp(buffer,"process")==0){
				findTopN(10);
				fp = fopen(filename,"r");
				if(fp==NULL){
				perror("[-]error in reading fie");
				exit(1);
				}
				// printf("send" );
				send_file(fp, newSocket);
				string message ="Bhai_sabse_jyda_cpu_use_bhejde";
				send(newSocket,message.c_str(),message.size(),0);
				// printf("send1" );
			}
			if(strcmp(buffer,":exit")==0){
				printf("connection Disconnected from %s:%d\n",inet_ntoa(newAddr.sin_addr),ntohs(newAddr.sin_port));
				break;
			 }
			 bzero(buffer,sizeof(buffer));
			 recv(newSocket,buffer,1024,0);

			 if(strcmp(buffer,"Bhej_Rha")){
			 	recv(newSocket,buffer,1024,0);
			 	printf("HCPU_usage %s",buffer);
			 }
			 // else{
			// 	// int k = stoi(buffer);
			// 	// findTopN(k);
			// 	// fp = fopen(filename,"r");
			// 	// if(fp==NULL){
			// 	// perror("[-]error in reading fie");
			// 	// exit(1);
			// 	// }
			// 	// send_file(fp, newSocket);
			// }
			
			// write_file(newSocket);
			// printf("[+]Data written in the file successfully.\n");

			// bzero(buffer,sizeof(buffer));
			// recv(newSocket,buffer,1024,0);

			// if(strcmp(buffer,":exit")==0){
			// 	printf("connection Disconnected from %s:%d\n",inet_ntoa(newAddr.sin_addr),ntohs(newAddr.sin_port));
			// 	break;
			// }

			// while(1){
			// 	bzero(buffer,sizeof(buffer));
			// 	// recv(newSocket,buffer,1024,0);

			// 	if(strcmp(buffer,":exit")==0){
			// 		printf("connection Disconnected from %s:%d\n",inet_ntoa(newAddr.sin_addr),ntohs(newAddr.sin_port));
			// 		break;
			// 	}else{
			// 		printf("client: %s\n",buffer);
			// 		// int k = stoi(buffer);
			// 		findTopN(5);
			// 		// fp = fopen(filename, "r");
  	// 		// 		if (fp == NULL) {
   //  	// 			perror("[-]Error in reading file.");
   //  	// 			exit(1);
  	// 		// 		}

  	// 				send_file(fp, newSocket);
  	// 				// printf("[+]File data sent successfully.\n");

			// 		/*fgets(buffer,255,stdin);
			// 		int n;
			// 		n= send(newSocket,buffer,strlen(buffer),0);
			// 		if(n<0){
			// 			printf("Error on writing");	
			// 		} 
			// 		int i = strncmp("bye",buffer,3);
			// 		if(i==0){
			// 			break;
			// 		}*/
			// 		// send(newSocket,buffer,strlen(buffer),0);
			// 		// bzero(buffer,sizeof(buffer));
			// 	}
			// }
		}	
	}
	close(newSocket);
	return 0;
}