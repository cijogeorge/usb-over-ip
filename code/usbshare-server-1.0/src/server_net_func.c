#include "../inc/usbshare_server.h"

/* my address information */
struct sockaddr_in my_addr;
/* connector’s address information */
struct sockaddr_in their_addr;
int sin_size;
int yes = 1;
 

void send_msg(int sockfd, char * sbuf)
{
 if(send(sockfd, sbuf, MAXDATASIZE-1 , 0) == -1)
 perror("send_msg()");
}

char * recv_msg(int sockfd)
{
 int numbytes;
 if((numbytes = recv(sockfd, rbuf, MAXDATASIZE-1, 0)) == -1)
 { 
  perror("recv_msg()");
  exit(1);
 }
 else  
 {
  rbuf[numbytes] = '\0';
  return rbuf;
 }
}

int sock_setup()
{
 int sockfd;
 /* listen on sock_fd, new connection on new_fd */
 if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
 {
  perror("socket()");
  exit(1);
 }
 
 else if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
 {
  perror("setsockopt()");
  exit(1);
 }
 /* host byte order */
 my_addr.sin_family = AF_INET;
 /* short, network byte order */
 my_addr.sin_port = htons(MYPORT);
 /* automatically fill with my IP */
 my_addr.sin_addr.s_addr = INADDR_ANY;
 /* zero the rest of the struct */
 memset(&(my_addr.sin_zero), '\0', 8);
 
 if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
 {
  perror("bind() error");
  exit(1);
 }
 
 else if(listen(sockfd, BACKLOG) == -1)
 {
  perror("Server-listen() error");
  exit(1);
 }
 
 else return sockfd;
}

int accept_con(int sockfd)
{
 int new_fd;
 sin_size = sizeof(struct sockaddr_in);
 if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
 {
  perror("accept()");
  exit(1);
 }
 else return new_fd;
}
