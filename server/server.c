#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

char userlist[10][10];
//char supernodes[]={"129.7.84.138"};
int count=0,num=0;
void error(const char *msg) {
    perror(msg);
    exit(1);
}

//supernode structures
struct supernode
{
  char ipaddress[20];
  char port[6];
};

struct supernode suprnods[4];
char supernodsocket[100];

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n,n1; /*dummies*/
    FILE *fp; /* file variable */
    char line[256];
    int cred,flag=0;
    time_t t; //new changes
    int k; char* temp; int tempport; //new changes

    if (argc < 2) {
         fprintf(stderr, "ERROR, no port provided\n");
         exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
         error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);


    //preparing supernode data

    strcpy(suprnods[0].ipaddress,"127.1.1.0");
    strcpy(suprnods[0].port, "8796");


    strcpy(suprnods[1].ipaddress,"125.1.1.0");
    strcpy(suprnods[1].port, "8908");


    strcpy(suprnods[2].ipaddress,"124.1.1.0");
    strcpy(suprnods[2].port, "8278");

    strcpy(suprnods[3].ipaddress,"126.1.1.0");
    strcpy(suprnods[3].port, "8453");

            printf("\nnumners  = \n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",suprnods[0].ipaddress,suprnods[1].ipaddress,suprnods[2].ipaddress,suprnods[3].ipaddress,suprnods[0].port,suprnods[1].port,suprnods[2].port,suprnods[3].port );

            // randomly select one Supernode using rand function

           /* k = rand() % 4;
            printf("Random = %d\n",k );
            printf("\nip = %s\n",suprnods[k].ipaddress );
            printf("\nport = %s\n",suprnods[k].port ); */

            strcat(supernodsocket,suprnods[0].ipaddress);
            strcat(supernodsocket," ");
            strcat(supernodsocket,suprnods[0].port);
            strcat(supernodsocket,"-");
            strcat(supernodsocket,suprnods[1].ipaddress);
            strcat(supernodsocket," ");
            strcat(supernodsocket,suprnods[1].port);
            strcat(supernodsocket,"-");
            strcat(supernodsocket,suprnods[2].ipaddress);
            strcat(supernodsocket," ");
            strcat(supernodsocket,suprnods[2].port);
            strcat(supernodsocket,"-");
            strcat(supernodsocket,suprnods[3].ipaddress);
            strcat(supernodsocket," ");
            strcat(supernodsocket,suprnods[3].port);

            printf("\nconcatenated = %s\n",supernodsocket);










    //handle multiple clients using fork

    int pid;
    while (1) {
         newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0)
                error("ERROR on accept");
         //fork new process
         pid = fork();
         if (pid < 0) {
              error("ERROR in new process creation");
         }
         if (pid == 0) {
            //child process
            //close(sockfd);
            //do whatever you want
            bzero(buffer, 256);
            n = read(newsockfd, buffer, 255);
            if (n < 0)
                error("ERROR reading from socket");
            printf("Login details from: %s\n", buffer);

            //user lists
          /*  strcat(userlist[count],buffer);
            count++;
            for(num=0;num<10;num++){
            printf("users:%s",userlist);
          } */
            //Authentication
            fp = fopen("users.txt", "r");
              while (fgets(line, sizeof(line),fp)) {
                if(flag==1){
                  break;
                }
                //printf("%s\n",line);
                cred = strncmp(buffer,line,strlen(line)-1);
                //printf("cred:%d", cred);
                //printf("%d",strlen(line)-1);
                if(cred == 0){
                  printf("Authenticated:");
                  flag = 1;
                }
                else{
                  printf("Invalid username/password");
                  flag = 0;
                  continue;
                }
          }
          fclose(fp);

            //appending user data into file
            if(flag == 0){
            fp = fopen("users.txt", "a");
            //while( (buf = getchar()) != EOF) {
            fprintf(fp,"%s\n",buffer);
            fclose(fp);
        }

        //selecting supernodes randomly

        //srand((unsigned) time(&t));

         // initializing supernodes address and port no

          //Send authentication acknoledgement to user
            if(flag==1){
            n1 = write(newsockfd,supernodsocket, 100);
            flag = 0;
          }
            else
            n1 = write(newsockfd, "Invalid username/password", 100);
            if (n < 0) {
                 error("ERROR writing to socket");
               close(newsockfd);
           }
           else {
             //parent process
             //close(newsockfd);
          }
    }

}
}
