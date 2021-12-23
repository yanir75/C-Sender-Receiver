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
// calculate time difference between two timeval in ms
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
// receives a file
void receive_file(int sock){
  int err;
  int num=0;
  struct timeval start,end;
  int count=0;
  gettimeofday(&start,0);
  char buffer[SIZE];
  float avg = 0;
	// receives the file until it can't receive.
  while (1) {
	  // err is the number of bytes it received , recv is for receiving from the sender
    err = recv(sock, buffer, 1024, 0);
    if (err <= 0){
      return;
      //printf("the numbers of bytes is:%d err",num);
    }
    num= num+ err;
	  // calculating time and size
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
	// the IP 
  char *ip = "127.0.0.1";
	// the port
  int port = 10000;
// my listening socket and the sender socket.
  int sock, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    perror("could not create socket");
    exit(1);
  }
  printf("socket was created\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);

  int err = bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(err < 0) {
    perror("could not bind socket");
    exit(1);
  }
  printf("socket was bound\n");

  if(listen(sock, 10) == 0){
		printf("listening\n");
	}else{
		perror("could not listen");
    exit(1);
	}

  addr_size = sizeof(new_addr);
  new_sock = accept(sock, (struct sockaddr*)&new_addr, &addr_size);
  if(new_sock ==-1){
	  printf("could not accept socket");
  }
  receive_file(new_sock);
  printf("The file was received successfully\n");
  close(new_sock);
  close(sock);
  return 0;
}
