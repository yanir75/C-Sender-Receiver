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
	  clock_t t;
  t = clock();
  int count=0;
  int size = FILESIZE*10;
  while (size>0) {
	  char * buffer = (char *) malloc(SIZE);
    err = recv(sockfd, buffer, SIZE, 0);
    if (err <= 0){
      break;
      return;
      //printf("the numbers of bytes is:%d",num);
    }
    size-=strlen(buffer);
    num= num+strlen(buffer);
	if(num>=FILESIZE){
		printf("the numbers of bytes is:%d \n",FILESIZE);
		count++;
		num=num-FILESIZE;
		  t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("The time it took to transfer the file is: %f \n", time_taken);
	t = clock();
	if(count==5){
		printf("Switching to reno\n");
	}
	}
    free(buffer);
  }
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
  printf("The file was received successfully");
	close(new_sock);
	close(sockfd);
  return 0;
}
