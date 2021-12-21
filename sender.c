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
void write_file(int sockfd){
  int err;
  int num=0;
	  clock_t t;
  t = clock();
  int count=0;
  while (1) {
	  char * buffer = (char *) malloc(FILESIZE);
    err = recv(sockfd, buffer, SIZE, 0);
    if (err <= 0){
      break;
      return;
      printf("the numbers of bytes is:%d",num);
    }
    num= num+strlen(buffer);
	if(num>=size){
		printf("the numbers of bytes is:%d \n",size);
		count++;
		num=num-size;
		  t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("The time it took to transfer the file is: %f \n", time_taken);
	t = clock();
	if(count==5){
		printf("Switching to reno");
	}
    free(buffer);
  }
  printf("the numbers of bytes is:%d \n",num);
  return;
}

int main(){
  char *ip = "127.0.0.1";
  int port = 10000;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];

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
  printf("The file was received successfully");

  return 0;
}
