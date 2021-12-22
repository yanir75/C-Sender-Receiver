#include <sys/socket.h> 
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#define FILESIZE 5000000
#define SIZE 1024
void write_file(int sockfd){
  int err;
  int num=0;
  time_t start,end;
  int count=0;
  int size = FILESIZE*10;
  time(&start);
  char buffer[SIZE];
  while (size>0) {
    err = recv(sockfd, buffer, 1024, 0);
    if (err <= 0){
      break;
      return;
      //printf("the numbers of bytes is:%d err",num);
    }
    size= size - err;
    num= num+ err;
	if(num>=FILESIZE){
		printf("the numbers of bytes is:%d \n",FILESIZE);
		count++;
		num=num-FILESIZE;
	time(&end);
	double time_taken=end-start;	
    printf("The time it took to transfer the file is: %f \n", time_taken);
	if(count==5){
		printf("Switching to reno\n");
	}
	time(&start);
	}
	bzero(buffer,SIZE);
  }
  printf("%d",count);
  return;
}

int main(){
  char *ip = "127.0.0.1";
  int port = 10000;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  int err = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(err < 0) {
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");

  if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		perror("[-]Error in listening");
    exit(1);
	}

  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  write_file(new_sock);
  printf("The file was received successfully\n");
  close(new_sock);
  close(sockfd);
  return 0;
}
