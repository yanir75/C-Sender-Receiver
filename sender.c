#include <sys/socket.h> 
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <arpa/inet.h>
//#define FILESIZE 5000000// around 4.8MB
#define SIZE 1024

//function to send the file, maybe because of the packet lose I should send in smaller sizes
void send_file(FILE *fp, int sockfd){
  int sentSize=0;
  char * data = (char *) malloc(SIZE);
  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data,strlen(data), 0) == -1) {
      perror("Could not send the file");
      exit(1);
    }
	// calculates the size 
	sentSize=sentSize+strlen(data);
	// frees the memory
    free(data);
	data = (char *) malloc(sizeof(char)*SIZE);
  }
  free(data);
  printf("%d bytes were sent\n",sentSize);
}

int main(int argc, char **argv) {

// buffer to configure whether it transmits in reno or cubic
char buf[256];
//a 32 bit int which we don't have to worry about giving the machine.
socklen_t len;
// sock creation kind inet meaning ipv4, sock_stream=TCP
int sock = socket(AF_INET, SOCK_STREAM, 0);
//if sock is -1 socket creation was not successfull
 	if (sock == -1) {
 		perror("socket");
 return -1;
 } 
//len is an int in the size of buff
len = sizeof(buf); 
// in our case it retrievies the TCP implementation, supposed to be cubic
if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) { 
perror("getsockopt");
return -1;
} 
// prints the TCP kind
printf("Current: %s\n", buf); 
// the IP i will use, send the information too
  char *ip = "127.0.0.1";
  // port i will use to send the information
  int port = 10000;
  // sockaddr of the server information.
  struct sockaddr_in serv;
  // AF_INET is ipv4 protocol
  serv.sin_family = AF_INET;
  // port equals the port we will use
  serv.sin_port = port;
  // convert ip to binary text 
  serv.sin_addr.s_addr = inet_addr(ip);
  // creates a connection to the server if err ==-1 connection couldn't be made
    int err = connect(sock, (struct sockaddr*)&serv, sizeof(serv));
      if(err == -1) {
    perror("Error could not connect\n");
    exit(1);
  }
  printf("Connection established\n");
  // just my filename
  char * filename = "1mb.txt";
  // opens the file
      FILE * fp;
    fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Could not open the file");
    exit(1);
  }
  // send the file 5 times
for(int i=1;i<=5;i++){
    send_file(fp, sock);
	//resets the pointer to the start of the file
    fseek(fp,0,SEEK_SET);
  printf("File number %d was sent successfully in Cubic\n",i);
}


// puts reno in the buff
strcpy(buf, "reno"); 
len = strlen(buf);
// change the socket to reno.
if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
perror("setsockopt"); 
return -1;
}
len = sizeof(buf);
// check the socket tcp implementation 
if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) {
perror("getsockopt"); 
return -1; 
}
// prints the socket tcp implementation 
printf("New: %s\n", buf);
for(int i=1;i<=5;i++){
        fseek(fp,0,SEEK_SET);
	send_file(fp, sock);
  printf("File number %d was sent successfully in reno\n",i);
}
// self explainatory
printf("closing connection and file\n");
fclose(fp);
close(sock); 
return 0; 
}
