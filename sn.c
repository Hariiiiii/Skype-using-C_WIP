#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

char buffer[256];
pthread_t thread;
char data[100];
char client_ip[] = {"129.7.84.138"};
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void error(const char *msg) {
    perror(msg);
    exit(1);
}

struct client
{
  char ipaddress[20];
  int port;
};

struct client clients[10];



void client_register(int sock){
    int newsockfd=sock,j=0,m;
    bzero(data,100);
    //recv(newsockfd, data,100,0);
    m = read(newsockfd, buffer, 255);
    if (m < 0)
        error("ERROR reading from socket");
    else
        printf("Everything fine here\n");
    printf("received data:%s",buffer);
    m = write(newsockfd,"registered",20);
    if (m < 0)
    error("ERROR writing to socket");
    close(newsockfd);
//pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd[1000], portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int no=0,n; /*dummies*/
    FILE *fp; /* file variable */


    if (argc < 2) {
         fprintf(stderr, "ERROR, no port provided\n");
         exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
         error("ERROR opening socket");
    printf("works\n");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(5679);
    printf("Hello\n");
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("ERROR on binding");
    }
      listen(sockfd, 5);
      printf("Hello2\n");
      printf("Everything fine\n");
      clilen = sizeof(cli_addr);
      while (1) {
        newsockfd[no] = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        char clntName[INET_ADDRSTRLEN];


if(inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,clntName,sizeof(clntName))!=NULL){

   strcpy(clients[no].ipaddress,clntName);
   printf("client ip %s",clients[no].ipaddress);

} else {
   printf("Unable to get address\n"); // i just fixed this to printf .. i had it as print before
}
         if (newsockfd[no] < 0)
                error("ERROR on accept");

                n = read(newsockfd[no], buffer, 255);
                printf("received before thread:%s",buffer);
                if (n < 0)
                    error("ERROR reading from socket");
                else
                    printf("Everything fine..2\n");

         printf("received before thread:%s",buffer);
         pthread_create(&thread,NULL,client_register,newsockfd[no]);
         printf("Thread no:%d\n",no);
        // getsockname(newsockfd[no], (struct sockaddr*)&cli_addr,&clilen);
        // clients[no].ipaddress = inet_ntoa(cli_addr.sin_addr);
        printf("beofer ip\n");

      //   printf("IP ADDRESS %s\n", inet_ntoa(cli_addr.sin_addr));

    //  printf("%s\n",cli_addr.sin_addr.s_addr&0xFF);




         char *token = strtok(buffer, "-");
         int i=0;
         while (token != NULL)
         {
           if (i%2==1)
           clients[no].port = strtol(token,NULL,10);
           token = strtok(NULL, "-");

           i+=1;
         }
         printf("portno %d\n", clients[no].port );


         no+=1;
        // n = write(sockfd,client_ip,strlen(client_ip)); //sending ip back to client
        // if (n < 0)
        // error("ERROR writing to socket");
        for(i=0;i<2;i++){
          printf("IP ADDRESS : %s\n",clients[i].ipaddress);
          printf("port : %d\n",clients[i].port);
        }
       }

         close(newsockfd[no]);
    close(sockfd);
  pthread_exit(NULL);
    //pthread_join(thread, NULL);
    //return 0;
     }
