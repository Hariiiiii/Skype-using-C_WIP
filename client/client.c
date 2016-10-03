#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int sockfd,sockfd1,sockfd2, newsockfd,newsockfd1, portno, n,choice;
struct sockaddr_in serv_addr, cli_addr;
struct hostent *server;
char message[100];
char buffer1[256];
socklen_t clilen;


void error(const char *msg)
{
    perror(msg);
    //exit(0);
}

struct supernode
{
  char ipaddress[20];
  int port;
};

struct supernode suprnods[4];


void listenmessage(int sock)

//send a message to client 2 from above ip

{
  sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd2 < 0)
      error("ERROR opening socket");
  printf("hello from the other side");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(5690);

  if (bind(sockfd2, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      error("ERROR on binding");
  listen(sockfd2, 5);
  clilen = sizeof(cli_addr);
  printf("Listening on port 5680\n");
  while (1) {
       newsockfd1 = accept(sockfd2, (struct sockaddr *) &cli_addr, &clilen);

       if (newsockfd1< 0)
              error("ERROR on accept");
              n = read(newsockfd1, message, 255);
              if (n < 0)
                  error("ERROR reading from socket");
              printf("Message from: %s\n", message);

}
close(newsockfd1);

}

void register1()
{
  int i;
  printf("entered register\n");
  char buffer2[256];
sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd1 < 0)
    error("ERROR opening socket");
//bzero(buffer,256);;
server = gethostbyname("172.29.3.17");


if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    //exit(0);
}
bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy((char *)server->h_addr,
     (char *)&serv_addr.sin_addr.s_addr,
     server->h_length);
serv_addr.sin_port = htons(5679);


//printf("h_addr: %s\n", inet_ntoa(serv_addr.sin_addr));

if (connect(sockfd1,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");
    //bzero(dummy,256);
    //printf("dummy %s", dummy);
    n = write(sockfd1,"RegisterMe-5690",20);
    if (n < 0)
    error("ERROR writing to socket");
    bzero(buffer2,256);
    while(1){
    n = read(sockfd1, buffer2, 255);

    if (n < 0)
        error("ERROR reading from socket");
    else{
      //  printf("Everything fine..2\n");
      printf("Received ack:%s",buffer2); // received client 2 ip

      close(sockfd1);
      break;
    }
  }

//  pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
    char uname[100];
    char pass[100];
    char nodedata[100];
    char nodeip[100];
    int i=0,j=0;
    char supernodeip[100][100];
    char supernodeip_temp[100];
    char selected_superip[100];
    char dummy[] = {"hellofromclient1"};
    pthread_t thread;
    pthread_t thread2;
    int node_choice=0;


    //char nodeip[5] = {};
    pthread_create(&thread,NULL,listenmessage,NULL);
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    //printf("server:%s", server);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        //exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    //printf("h_addr: %s\n", inet_ntoa(serv_addr.sin_addr));
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
        bzero(buffer,256);
        //user details

    /*printf("----------------SKYPE--------------------\n");
    printf("1.Login\n2.Chat\n3.Exit\n");
    printf("Enter choice:");
    scanf("%d",&choice);
    switch (choice) {
      case 1 :
           {*/
           {
            printf("Please enter username: ");
            gets(uname);
            //fgets(uname, BUFSIZE, stdin);
            printf("Please enter password: ");
            gets(pass);
            //fgets(pass, BUFSIZE, stdin);
            strcat(uname," ");
            strcat(uname,pass);
            strcat(buffer,uname);

            //fgets(buffer,255,stdin);
            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0)
            error("ERROR writing to socket");
            bzero(buffer,256);
            n = read(sockfd,buffer,255);
            if (n < 0)
            error("ERROR reading from socket");
            printf("%s\n",buffer);  //supernode ip from server
            bzero(nodeip,100);
            strcat(nodeip,buffer);
            //printf("before close");
            close(sockfd);
            //printf("after close");


            //splitting supernodes based on delimeter
            char *token = strtok(buffer, "-"); char tempport[7];
            while (token != NULL && i<4)
            {

              strcpy(supernodeip_temp,token);
              strcpy(supernodeip[i],supernodeip_temp);

              token = strtok(NULL, "-");
              printf("supernodes %s\n",supernodeip[i]);
              i+=1;
            }

          }

          // storing supernode and port in structure
          char super_temp[20];
          while(j<4){
          strcpy(super_temp,supernodeip[j]);
          char *token1 = strtok(super_temp, " ");

          strcpy(suprnods[j].ipaddress,token1);
          i=0;
          while (token1 != NULL)
          {
            if (i%2==1)
              suprnods[j].port = strtol(token1,NULL,10);
            token1 = strtok(NULL, " ");
      //      printf("inside while1\n %s",token1);
            i++;
          }

          printf("supernode ip struct %s\n",suprnods[j].ipaddress);
          printf("supernode port struct %d\n",suprnods[j].port);

          j+=1;

       }
          //Let user decide his supernode
          printf("Choose the supernode:\n");
          for(i=0;i<4;i++){
            printf("%d%s\n",i+1,supernodeip[i]);
          }
          scanf("%d\n",&node_choice);
          switch(node_choice) {
            case 1:
                    //register1(suprnods[0].ipaddress,suprnods[0].port);
                    pthread_create(&thread2,NULL,register1,NULL);
                    printf("creating thread......\n:");
                    break;
            case 2:
                  //  register(suprnods[1].ipaddress,suprnods[1].port);
            case 3:
                  //  register(suprnods[2].ipaddress,suprnods[2].port);
            default:
                    break;
                  //  register(suprnods[3].ipaddress,suprnods[3].port);
          }

/*
//connecting to supernodes for getting other client ip
printf("hello");
{
sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd1 < 0)
    error("ERROR opening socket");
printf("hello from the other side");
//bzero(buffer,256);
printf("supernodeip%s\n",nodeip);
server = gethostbyname(nodeip);
printf("data %s%s",dummy,nodeip);

if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    //exit(0);
}
bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy((char *)server->h_addr,
     (char *)&serv_addr.sin_addr.s_addr,
     server->h_length);
serv_addr.sin_port = htons(5679);


//printf("h_addr: %s\n", inet_ntoa(serv_addr.sin_addr));

if (connect(sockfd1,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");
    //bzero(dummy,256);
    printf("dummy %s", dummy);
    n = write(sockfd1,dummy,strlen(dummy));
    if (n < 0)
    error("ERROR writing to socket");
    bzero(buffer1,256);
    while(1){
    n = read(sockfd1, buffer1, 255);

    if (n < 0)
        error("ERROR reading from socket");
    else{
      //  printf("Everything fine..2\n");
      printf("Received IP:%s",buffer1); // received client 2 ip
      close(sockfd1);
      break;
    }
      }
} */
pthread_exit(NULL);
    return 0;
}
