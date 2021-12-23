#include <sys/socket.h> 
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>
#define FILESIZE 5000000
#define SIZE 1024

float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

void receive_file(int sockfd){
  int err;
  int num=0;
  struct timeval start,end;
  int count=0;
  int size = FILESIZE*10;
  gettimeofday(&start,0);
  char buffer[SIZE];
  float avg = 0;
  while (1) {
    err = recv(sockfd, buffer, 1024, 0);
    if (err <= 0){
      return;
      //printf("the numbers of bytes is:%d err",num);
    }
    num= num+ err;
	if(num>=FILESIZE){
		printf("the numbers of bytes is:%d \n",FILESIZE);
		count++;
		num=num-FILESIZE;
	gettimeofday(&end, 0);
	float time_per_file=timedifference_msec(start,end);	
	avg+=time_per_file*0.001;
    printf("The time it took to transfer the file is: %f seconds \n", time_per_file*0.001);
	if(count==5){
		printf("avg response time is: %f\n",avg/5);
		avg=0;
		printf("sender is switching to reno\n");
	}
	gettimeofday(&start,0);
	}
	bzero(buffer,SIZE);
  }
  printf("avg response time is: %f\n",avg/5);
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
    perror("could not create socket");
    exit(1);
  }
  printf("socket was created\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  int err = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(err < 0) {
    perror("could not bind socket");
    exit(1);
  }
  printf("socket was bound\n");

  if(listen(sockfd, 10) == 0){
		printf("listening\n");
	}else{
		perror("could not listen");
    exit(1);
	}

  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  if(new_sock ==-1){
	  printf("could not accept socket");
  }
  receive_file(new_sock);
  printf("The file was received successfully\n");
  close(new_sock);
  close(sockfd);
  return 0;
}
